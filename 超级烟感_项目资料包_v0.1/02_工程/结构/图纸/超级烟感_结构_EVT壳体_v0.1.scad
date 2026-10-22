// 超级烟感 · 结构 · EVT 壳体 v0.1
// Issue #17（保持打开）。本文件是壳体几何初稿，不是模具图，不是 UL/EN 符合性模型。
//
// 坐标系（与结构草排 / G3 装配图一致）：
//   原点 = 板心 = 壳体圆心；单位 mm
//   正视外观面：+X 右，+Y 上
//   PCB 顶面铜 Z = 0；+Z 朝房间（天花安装时朝下）
//   外观外表面 Z = +26；底座外表面 Z = -16；总厚 42
//
// 锁定几何：
//   外盘 Ø135，厚 42
//   螺柱 R=41.6：(41.6, 0)、(-20.8, +36.026)、(-20.8, -36.026)
//     G3 Markdown 把 Y 写成 ±36.0，与本模型差 0.026 mm，在 EVT ±0.5 内
//   灯环中心线 R=58，ch0–63 共 64 个细柱孔（正视逆时针）。不是 12 点，不是 Nest 大环面
//   天线窗 (0, +52)，开口 18×12；内部净空柱 Ø20、Z 1.8→6.8（高 5）
//   白壳薄盘：主壁 1.8，空腔，不是实心饼
//
// 重出网格：
//   openscad -o 超级烟感_结构_EVT壳体_v0.1.stl 超级烟感_结构_EVT壳体_v0.1.scad
// OpenSCAD 2021 不能写 STEP。同目录 .step 用 OpenCASCADE 按本文件尺寸重建。
// 拔模约 1° 只写在材料壁厚表，不写进本网格，避免改掉 Ø135 / 厚 42。

/* [locked] */
od             = 135;
thickness      = 42;
z_face         = 26;
z_base         = -16;
wall           = 1.8;

post_r         = 41.6;
post_od        = 4.5;     // DFM：外径约 4.5。根部 R0.4 圆角留待开模，本网格不做
post_pilot_d   = 1.6;     // M2 自攻前孔初值。螺钉牌号未定，孔径 待补
post_support_z = -1;      // PCB 支撑面
post_hole_floor_left = 1.2; // 孔底距内底面，不穿出 Z=-16 外皮
// 120° 三点。Y 取 36.026（用户锁定的三位小数），不用已作废的 R=48。
post_xy = [
  [ 41.6,   0.000],
  [-20.8,  36.026],
  [-20.8, -36.026]
];

led_n    = 64;            // ch0–ch63。禁止 12 灯点，禁止整圈光槽
led_r    = 58;
led_d    = 1.4;           // 与细缝径向 1.4 同量级的柱孔，沉通顶壁
led_step = 360 / led_n;   // 5.625°

antenna_xy      = [0, 52];
antenna_win     = [18, 12]; // 中心 (0,+52)：X -9..+9，Y +46..+58
antenna_keep_d  = 20;       // 草排/DFM「径约 20」按直径 Ø20
antenna_keep_z0 = 1.8;
antenna_keep_z1 = 6.8;      // 高度 5.0 = 馈点上方净空
keepout_min     = 5;

status_d = 2.4;             // 中心状态点。不是按键，不是环面

eps      = 0.05;
fn_outer = 96;
fn_small = 24;

assert(thickness <= 42);
assert(abs((z_face - z_base) - thickness) < 1e-9);
assert(led_n == 64);
assert(abs(led_step - 5.625) < 1e-9);
assert(antenna_keep_z1 - antenna_keep_z0 >= keepout_min - 1e-9);
assert(len(post_xy) == 3);

module assert_posts_on_radius() {
  for (p = post_xy)
    assert(abs(norm([p[0], p[1]]) - post_r) < 0.01);
}
assert_posts_on_radius();

// 窗缘到内圆柱壁的最小间隙（+Y 与窗角）。侧壁是「另一道壁」，不是窗洞自身的棱。
inner_r = od / 2 - wall;
win_y_hi = antenna_xy[1] + antenna_win[1] / 2;
echo(str("inner_r=", inner_r));
echo(str("window_+Y_edge_to_inner_wall=", inner_r - win_y_hi));
echo(str("keepout_cylinder_+Y_to_inner_wall=", inner_r - (antenna_xy[1] + antenna_keep_d / 2)));

module shell_body() {
  difference() {
    translate([0, 0, z_base])
      cylinder(h = thickness, r = od / 2, $fn = fn_outer);
    // 顶、底、侧壁都留 wall。空腔使外观是薄盘而不是实心厚饼。
    translate([0, 0, z_base + wall])
      cylinder(h = thickness - 2 * wall, r = inner_r, $fn = fn_outer);
  }
}

module screw_posts() {
  h = post_support_z - (z_base + wall);
  for (p = post_xy)
    translate([p[0], p[1], z_base + wall - eps])
      cylinder(h = h + eps, d = post_od, $fn = fn_small);
}

module screw_holes() {
  z0 = z_base + wall + post_hole_floor_left;
  h  = post_support_z - z0;
  for (p = post_xy)
    translate([p[0], p[1], z0])
      cylinder(h = h + eps, d = post_pilot_d, $fn = fn_small);
}

// ch_k：α = k × 5.625°，自 +X 逆时针。X = 58·cos α，Y = 58·sin α。
module led_holes() {
  for (k = [0:led_n - 1]) {
    a = k * led_step;
    translate([led_r * cos(a), led_r * sin(a), z_face - wall - eps])
      cylinder(h = wall + 2 * eps, d = led_d, $fn = fn_small);
  }
}

module center_status_hole() {
  translate([0, 0, z_face - wall - eps])
    cylinder(h = wall + 2 * eps, d = status_d, $fn = fn_small);
}

module antenna_window() {
  translate([antenna_xy[0], antenna_xy[1], z_face - wall / 2])
    cube([antenna_win[0], antenna_win[1], wall + 2 * eps], center = true);
}

// 净空柱在空腔内部。柱内不得再长螺柱、肋、镶针。
// 本柱完全落在内腔里，不切削外皮；窗洞另走 antenna_window()。
module antenna_keepout() {
  translate([antenna_xy[0], antenna_xy[1], antenna_keep_z0])
    cylinder(h = antenna_keep_z1 - antenna_keep_z0, d = antenna_keep_d, $fn = fn_outer);
}

module evt_shell() {
  difference() {
    union() {
      shell_body();
      screw_posts();
    }
    screw_holes();
    led_holes();
    center_status_hole();
    antenna_window();
    antenna_keepout();
  }
}

color("white") evt_shell();
