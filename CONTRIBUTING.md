# 🤝 Contributing to the Embedded Engineering Roadmap

Thank you for your interest in improving the **Embedded Engineering Roadmap**! This repository is an open community resource dedicated to providing free, high-caliber education for aspiring and practicing embedded systems engineers.

---

## 🎯 Ways You Can Contribute

1. **Fixing Typos & Clarifying Concepts:** Improving documentation, grammar, or technical explanations.
2. **Adding Code Examples:** Submitting new bare-metal, RTOS, or peripheral driver snippets in standard C/C++.
3. **Recommending Quality Resources:** Suggesting books, simulators, blogs, or courses that meet our quality evaluation criteria.
4. **Hardware Verification:** Testing existing project code on different silicon revisions or development boards (Nucleo, Discovery, Black Pill, ESP32) and documenting findings.

---

## 📜 Contribution Guidelines

- **Code Quality:** All C code must follow the C99 or C11 standard, compile with `-Wall -Wextra -Werror`, avoid compiler-specific non-standard extensions without guards, and include descriptive comments.
- **Resource Recommendations:** Please submit resource suggestions using our [Resource Suggestion Issue Template](.github/ISSUE_TEMPLATE/resource_suggestion.md) with our standard 4-dimension rating rubric.
- **Git Commit Etiquette:** Use clear, conventional commit messages:
  - `feat: add SPI DMA ring buffer driver example`
  - `docs: update Cortex-M4 vector table cheatsheet`
  - `fix: correct I2C pull-up formula in cheatsheet`

---

## 🛠️ Development & Submission Process

1. **Fork the repository** on GitHub.
2. **Create a topic branch** from `main`:
   ```bash
   git checkout -b feature/new-peripheral-driver
   ```
3. **Commit your modifications** with concise messages.
4. **Push to your fork** and submit a Pull Request.
5. Ensure your PR description explains the rationale and includes any hardware testing logs.
