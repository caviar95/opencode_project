# auto-pipeline

C++ 自动化流水线示例工程：一个带完整 CI/CD 流程的现代 CMake 项目模板。

## 特性

- **构建**：CMake ≥ 3.20，支持 `CMakePresets.json` 一键配置（dev / ci）
- **测试**：CTest 单元测试（自包含、零外部依赖）
- **静态分析**：`clang-tidy`（`WarningsAsErrors`，问题视为错误）
- **格式检查**：`clang-format`（GitHub Actions 强制门禁）
- **Sanitizers**：ASan + UBSan 构建
- **CI**：GitHub Actions 三平台矩阵（Linux / macOS / Windows）+ 独立 lint / format / sanitizer 任务
- **发布**：打 `v*` tag 自动构建、测试并发布 Release 产物

## 目录结构

```
.
├── CMakeLists.txt            # 顶层构建脚本
├── CMakePresets.json         # dev(带sanitizer) / ci 预设
├── .clang-format             # 代码格式规范
├── .clang-tidy               # 静态检查规则
├── .github/workflows/
│   ├── ci.yml                # 每次 push/PR 触发
│   └── release.yml           # 打 v* tag 时触发
├── include/auto_pipeline/    # 公共头文件
├── src/                      # 实现 + 可执行文件
├── tests/                    # 单元测试
└── scripts/                  # 本地自动化脚本
```

## 本地使用

```bash
# 一键流水线（format → lint → 构建 → 测试）
./scripts/check_format.sh          # 格式门禁
./scripts/check_lint.sh            # 静态检查
cmake --preset dev && cmake --build --preset dev
ctest --test-dir build/dev --output-on-failure

# 自动修复格式
./scripts/check_format.sh --fix

# 运行示例程序
./build/dev/auto_demo add 2 3
./build/dev/auto_demo div 9 3
```

## 流水线各阶段

| 阶段 | 工具 | 门禁方式 |
|------|------|----------|
| 格式 | `clang-format` | `--dry-run --Werror`，不通过则 CI 失败 |
| 静态分析 | `clang-tidy` | `.clang-tidy` 中 `WarningsAsErrors: '*'` |
| 构建 | CMake | 三平台 Release |
| 测试 | CTest | 失败即流水线失败 |
| 内存安全 | ASan/UBSan | Linux 专用 job |
| 发布 | GitHub Release | tag 触发，打包二进制 |

## CI 流程

`.github/workflows/ci.yml` 在 push/PR 到 `main` 时并行执行：

```
format-check ─┐
lint          ─┤
build ×3 OS   ─┼─→ 全部通过才可合并
sanitizers    ─┘
```

发布流程（`release.yml`）：打 `git tag v1.0.0 && git push --tags`，
自动构建、测试、打包 `auto-pipeline-<version>-linux-x64.tar.gz` 并创建 GitHub Release。
