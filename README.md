# test1

超级烟感资料与固件的入口。

| 入口 | 路径 |
|---|---|
| 资料包总索引 | [超级烟感_项目资料包_v0.1/README.md](超级烟感_项目资料包_v0.1/README.md) |
| 软件设计包（详设 / 索引 / 缺口） | [超级烟感_项目资料包_v0.1/02_工程/软件/README.md](超级烟感_项目资料包_v0.1/02_工程/软件/README.md) |
| 可执行固件源码 | [`firmware/src/`](firmware/src) · 说明 [`firmware/README.md`](firmware/README.md) |
| ICD 现行版 | [超级烟感_项目资料包_v0.1/02_工程/超级烟感_软件硬件_ICD_v0.1.7.md](超级烟感_项目资料包_v0.1/02_工程/超级烟感_软件硬件_ICD_v0.1.7.md) |
| nRF54L15 镜像与 EVT 电流样 | [Issue #15](https://github.com/Leo2442926161/test1/issues/15)（未关闭） |

`firmware/` 中的 `fw-hush-relay-0.1.0-evt` 是宿主可测行为模型（`image=host-sim`）。烧录镜像仍由 Issue #15 交付。

硬件约定目录 `02_工程/硬件设计/`、结构约定目录 `02_工程/结构/`、测试约定目录 `04_测试/` 由对应岗位维护。

```sh
make -C firmware test
```
