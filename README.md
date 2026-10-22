# test1

可测固件包（ICD v0.1.7 hush / 级联 / SED）在 [`firmware/`](firmware/README.md)。

```sh
make -C firmware test
```

资料包文档在 PR #1，不并入本分支。固件单独放在顶层 `firmware/`，G3 克隆本 PR 即可跑钩子，不必等资料包合并，也不必等 nRF54L15 烧录环境。
