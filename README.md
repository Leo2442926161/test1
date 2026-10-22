# test1

超级烟感的资料和固件在这一个仓库里。GitHub 根上是三样：本 README、`firmware/`、`超级烟感_项目资料包_v0.1/`。

`firmware/` 留在仓顶是脊骨约定：归软件，方便 `make` 和 CI。它属于本项目，不是资料包外面的旁路。不要把它搬进资料包。里面的 `fw-hush-relay-0.1.0-evt` 是宿主 sim（`image=host-sim`），不是 nRF54L15 烧录镜像。

## 从这里进

| 要看的东西 | 路径 |
|---|---|
| 资料包总索引 | [超级烟感_项目资料包_v0.1/README.md](超级烟感_项目资料包_v0.1/README.md) |
| 人员职责与工作流总表 | [超级烟感_项目资料包_v0.1/超级烟感_人员职责与工作流_v0.1.md](超级烟感_项目资料包_v0.1/超级烟感_人员职责与工作流_v0.1.md) |
| 软件 | [超级烟感_项目资料包_v0.1/02_工程/软件/README.md](超级烟感_项目资料包_v0.1/02_工程/软件/README.md) |
| 硬件 | [超级烟感_项目资料包_v0.1/02_工程/硬件设计/README.md](超级烟感_项目资料包_v0.1/02_工程/硬件设计/README.md) |
| 结构 | [超级烟感_项目资料包_v0.1/02_工程/结构/README.md](超级烟感_项目资料包_v0.1/02_工程/结构/README.md) |
| 测试 | [超级烟感_项目资料包_v0.1/04_测试/README.md](超级烟感_项目资料包_v0.1/04_测试/README.md) |
| 固件宿主说明 | [`firmware/README.md`](firmware/README.md) |
| nRF 骨架 | [`firmware/nrf/README.md`](firmware/nrf/README.md)（`ncs_board_build=false`，无烧录镜像） |
| 实机镜像与 EVT 电流样 | [Issue #15](https://github.com/Leo2442926161/test1/issues/15)（未关闭） |

```sh
make -C firmware test
```
