# MIXR (Mixed Reality Simulation Platform) Learning Repository

This repository serves as a personal learning environment for exploring the **MIXR (Mixed Reality Simulation Platform)** framework, formerly known as OpenEaagles. MIXR is a powerful, government-developed **open-source framework** designed for creating high-performance simulation applications.

For more information on the MIXR project, visit its official GitHub repository: [https://github.com/doughodson/mixr](https://github.com/doughodson/mixr)

---

## Installation and Setup Guide

This section details the steps taken to install and set up the MIXR framework, allowing others to replicate the development environment.

1.  **Download MIXR Components**:
    I downloaded the following components from the official MIXR downloads page ([https://www.mixr.dev/downloads.html](https://www.mixr.dev/downloads.html)):
    * Version 18.01 Framework
    * Version 18.01 Data
    * Version 18.01 Examples
    * Version 18.01 3rd Party Libraries

2.  **Organize Directories**:
    After unzipping all downloaded folders, I organized them into a single, larger **MIXR** parent folder. The structure for the framework, examples, and 3rd party libraries is reflected in this repository. Due to their large file sizes, the `data` files were kept separate from the main repository.

3.  **Install Visual Studio 2022**:
    **Visual Studio 2022** was installed as the Integrated Development Environment (IDE) for building and working with the MIXR framework.

4.  **Build Process in Visual Studio**:
    To achieve a clean and successful build, the solution files for each component had to be loaded and built in a specific order due to their dependencies:
    * I navigated to the `build` folder within each component's directory (except for the `data` folder).
    * I opened the respective **solution file (.sln)** in Visual Studio 2022.
    * The build order was critical:
        1.  **3rd Party Libraries**: Built first, as they are foundational dependencies for MIXR.
        2.  **MIXR Framework**: Built next, as it depends on the 3rd Party Libraries.
        3.  **MIXR Examples**: Built last, as they depend on both the MIXR Framework and the 3rd Party Libraries.
    * For each solution, I initiated the build process by going to `Build > Build Solution` in the Visual Studio menu, or by using the hotkey `Ctrl + Shift + B`.

This methodical build process ensured that all necessary dependencies were met, resulting in a successful compilation of the entire framework and its examples.

---

## Repository Objectives

The primary objective of this repository is to serve as a hands-on learning platform for the MIXR framework, ultimately aiming to enhance my competitive edge in the job market. This pursuit allows me to:

* **Hone Object-Oriented Programming (OOP) skills**: MIXR's architecture heavily relies on OOP principles, providing a practical environment for reinforcement.
* **Refresh Software Design Patterns knowledge**: Working with the framework offers opportunities to recognize and apply various software design patterns in a real-world context.
* **Practice C++**: MIXR is built entirely in C++, offering extensive practice with the language's intricacies and modern features.
* **Learn Simulation Techniques and Practices**: The framework provides direct exposure to the methodologies and best practices involved in developing complex simulation applications.

While there isn't a definitive end goal project for this repository, I plan to develop a few custom example simulations to solidify my understanding and explore the framework's capabilities further.
