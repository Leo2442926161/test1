#include "fw.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sim {
    fw_node_t a;
    fw_node_t b;
    uint32_t b_applied_seq;
} sim_t;

static void sim_init(sim_t *s)
{
    fw_node_init(&s->a);
    fw_node_init(&s->b);
    s->b_applied_seq = 0;
}

static void usage(void)
{
    fprintf(stderr,
            "usage: fw_sim [--state FILE] <command> [args]\n"
            "commands:\n"
            "  reset | status\n"
            "  smoke | co | clear | mute | critical | peer-mute\n"
            "  tick N | ux-tick N | hush-seconds N\n"
            "  relay-enable 0|1 | peer-relay-enable 0|1\n"
            "  cloud 0|1 | matter 0|1 | tx-fail 0|1 | voice 0|1\n"
            "  group ID | peer-group ID | pair\n"
            "  hop\n"
            "  ux MODE WINDOW BRIGHTNESS SEGMENTS FLAGS\n"
            "FLAGS: 0 | rf | factory | rf,factory\n"
            "MODE: idle ring_sweep path_guide pulse_smoke pulse_co mute_hold full_white\n"
            "stdout is one JSON status object (DUT = node A, peer = node B).\n");
}

static int parse_01(const char *s, bool *out)
{
    if (strcmp(s, "0") == 0) {
        *out = false;
        return 0;
    }
    if (strcmp(s, "1") == 0) {
        *out = true;
        return 0;
    }
    return -1;
}

static int parse_i(const char *s, long *out)
{
    char *end = NULL;
    long v;
    errno = 0;
    v = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0') {
        return -1;
    }
    *out = v;
    return 0;
}

static fw_ux_mode_t parse_mode(const char *s)
{
    if (strcmp(s, "idle") == 0) {
        return FW_UX_IDLE;
    }
    if (strcmp(s, "ring_sweep") == 0) {
        return FW_UX_RING_SWEEP;
    }
    if (strcmp(s, "path_guide") == 0) {
        return FW_UX_PATH_GUIDE;
    }
    if (strcmp(s, "pulse_smoke") == 0) {
        return FW_UX_PULSE_SMOKE;
    }
    if (strcmp(s, "pulse_co") == 0) {
        return FW_UX_PULSE_CO;
    }
    if (strcmp(s, "pulse_fault") == 0) {
        return FW_UX_PULSE_FAULT;
    }
    if (strcmp(s, "mute_hold") == 0) {
        return FW_UX_MUTE_HOLD;
    }
    if (strcmp(s, "full_white") == 0) {
        return FW_UX_FULL_WHITE;
    }
    return (fw_ux_mode_t)-1;
}

static uint32_t parse_flags(const char *s)
{
    uint32_t flags = 0;
    if (strstr(s, "factory") != NULL) {
        flags |= FW_UX_FLAG_FACTORY;
    }
    if (strstr(s, "rf") != NULL) {
        flags |= FW_UX_FLAG_RF_PEAK;
    }
    return flags;
}

static int exec_cmd(sim_t *s, int argc, char **argv)
{
    bool bit = false;
    long num = 0;

    if (argc < 1) {
        return -1;
    }
    if (strcmp(argv[0], "reset") == 0 && argc == 1) {
        sim_init(s);
        return 0;
    }
    if (strcmp(argv[0], "status") == 0 && argc == 1) {
        return 0;
    }
    if (strcmp(argv[0], "smoke") == 0 && argc == 1) {
        return fw_set_alarm(&s->a, FW_ALARM_SMOKE);
    }
    if (strcmp(argv[0], "co") == 0 && argc == 1) {
        return fw_set_alarm(&s->a, FW_ALARM_CO);
    }
    if (strcmp(argv[0], "clear") == 0 && argc == 1) {
        fw_clear_alarm(&s->a);
        return 0;
    }
    if (strcmp(argv[0], "mute") == 0 && argc == 1) {
        /* Domain reject (CO) is a status result, not a CLI failure. */
        fw_mute(&s->a);
        return 0;
    }
    if (strcmp(argv[0], "peer-mute") == 0 && argc == 1) {
        fw_mute(&s->b);
        return 0;
    }
    if (strcmp(argv[0], "critical") == 0 && argc == 1) {
        fw_critical(&s->a);
        return 0;
    }
    if (strcmp(argv[0], "pair") == 0 && argc == 1) {
        fw_set_group(&s->a, 1);
        fw_set_group(&s->b, 1);
        return 0;
    }
    if (strcmp(argv[0], "hop") == 0 && argc == 1) {
        if (fw_tx_seq(&s->a) == 0 || fw_tx_seq(&s->a) == s->b_applied_seq) {
            return 0;
        }
        fw_alarm_relay_inject_rx(&s->b, fw_last_tx_type(&s->a), fw_group(&s->a));
        s->b_applied_seq = fw_tx_seq(&s->a);
        return 0;
    }
    if (argc == 2 && parse_i(argv[1], &num) == 0) {
        if (strcmp(argv[0], "tick") == 0 && num >= 0) {
            fw_tick(&s->a, (uint32_t)num);
            fw_tick(&s->b, (uint32_t)num);
            return 0;
        }
        if (strcmp(argv[0], "ux-tick") == 0 && num >= 0) {
            fw_ux_tick(&s->a, (uint32_t)num);
            return 0;
        }
        if (strcmp(argv[0], "hush-seconds") == 0) {
            fw_set_hush_smoke_s(&s->a, (int)num);
            return 0;
        }
        if (strcmp(argv[0], "group") == 0 && num >= 0) {
            fw_set_group(&s->a, (uint32_t)num);
            return 0;
        }
        if (strcmp(argv[0], "peer-group") == 0 && num >= 0) {
            fw_set_group(&s->b, (uint32_t)num);
            return 0;
        }
    }
    if (argc == 2 && parse_01(argv[1], &bit) == 0) {
        if (strcmp(argv[0], "relay-enable") == 0) {
            fw_nvs_set_relay_enable(&s->a, bit);
            return 0;
        }
        if (strcmp(argv[0], "peer-relay-enable") == 0) {
            fw_nvs_set_relay_enable(&s->b, bit);
            return 0;
        }
        if (strcmp(argv[0], "cloud") == 0) {
            fw_set_cloud_up(&s->a, bit);
            return 0;
        }
        if (strcmp(argv[0], "matter") == 0) {
            fw_set_matter_session(&s->a, bit);
            return 0;
        }
        if (strcmp(argv[0], "tx-fail") == 0) {
            fw_set_tx_fail(&s->a, bit);
            return 0;
        }
        if (strcmp(argv[0], "voice") == 0) {
            fw_set_voice_enable(&s->a, bit);
            return 0;
        }
    }
    if (strcmp(argv[0], "ux") == 0 && argc == 6) {
        fw_ux_mode_t mode = parse_mode(argv[1]);
        long window = 0;
        long bright = 0;
        long segs = 0;
        if (mode == (fw_ux_mode_t)-1) {
            return -1;
        }
        if (parse_i(argv[2], &window) != 0 || parse_i(argv[3], &bright) != 0 ||
            parse_i(argv[4], &segs) != 0) {
            return -1;
        }
        fw_ux_set(&s->a, mode, (int)window, (int)bright, (int)segs, parse_flags(argv[5]));
        return 0;
    }
    fprintf(stderr, "unknown command\n");
    return -1;
}

static int split_line(char *line, char **argv, int max)
{
    int n = 0;
    char *p = line;
    while (*p != '\0' && n < max) {
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        if (*p == '\0' || *p == '\n' || *p == '#') {
            break;
        }
        argv[n++] = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n') {
            p++;
        }
        if (*p != '\0') {
            *p = '\0';
            p++;
        }
    }
    return n;
}

static int replay(sim_t *s, const char *path)
{
    FILE *fp = fopen(path, "r");
    char line[256];
    if (fp == NULL) {
        return 0;
    }
    while (fgets(line, sizeof line, fp) != NULL) {
        char *argv[8];
        int argc = split_line(line, argv, 8);
        if (argc == 0) {
            continue;
        }
        if (exec_cmd(s, argc, argv) != 0) {
            fprintf(stderr, "state replay failed: %s\n", argv[0]);
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

static int append_cmd(const char *path, int argc, char **argv)
{
    FILE *fp = fopen(path, "a");
    int i;
    if (fp == NULL) {
        perror(path);
        return -1;
    }
    for (i = 0; i < argc; i++) {
        if (i > 0) {
            fputc(' ', fp);
        }
        fputs(argv[i], fp);
    }
    fputc('\n', fp);
    if (fclose(fp) != 0) {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *state_path = "fw.state";
    int base = 1;
    sim_t sim;
    char json[8192];
    int rc;

    if (argc >= 3 && strcmp(argv[1], "--state") == 0) {
        state_path = argv[2];
        base = 3;
    }
    if (base >= argc) {
        usage();
        return 2;
    }

    sim_init(&sim);
    if (strcmp(argv[base], "reset") == 0) {
        FILE *fp = fopen(state_path, "w");
        if (fp == NULL) {
            perror(state_path);
            return 1;
        }
        fputs("reset\n", fp);
        fclose(fp);
    } else if (replay(&sim, state_path) != 0) {
        return 1;
    }

    rc = exec_cmd(&sim, argc - base, argv + base);
    if (rc == 0 && strcmp(argv[base], "status") != 0 && strcmp(argv[base], "reset") != 0) {
        if (append_cmd(state_path, argc - base, argv + base) != 0) {
            return 1;
        }
    }
    if (fw_status_json(&sim.a, &sim.b, json, sizeof json) != 0) {
        fprintf(stderr, "status buffer too small\n");
        return 1;
    }
    puts(json);
    return rc == 0 ? 0 : 2;
}
