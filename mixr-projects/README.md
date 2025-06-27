# mixr-projects

This directory contains custom projects built on top of the [MIXR](https://github.com/Mixer-Sim/mixr) simulation framework. The setup process to create these projects was non-trivial and is documented below for future reference or reuse.

---

## 🛠 Project Setup Instructions

Each new project in this directory (starting with `templateProject`) was configured using the following steps:

### 1. **Base Structure**
- Cloned the structure of `mixr-examples`.
- Copied the `/lib` and `/share` directories from `mixr-examples` into the `mixr-projects` directory (may not be strictly necessary, but done for structural consistency).

### 2. **Project Template**
- Created a project named `templateProject` which acts as a base for new projects.
- It contains a `main.cpp` file which replicates the `tutorial01` "Hello World" example from `mixr-examples`.

### 3. **Project Configuration**

Inside the Visual Studio project properties of `templateProject`:

####   ➤ **C/C++ > General > Additional Include Directories**
```

../../mixr/include;../../mixr-3rdparty/include;%(AdditionalIncludeDirectories)

```

####   ➤ **Linker > General > Additional Library Directories**
```

../../mixr/lib;../../mixr-3rdparty/lib/vs2017-32;%(AdditionalLibraryDirectories)

```

####   ➤ **Linker > Input > Additional Dependencies**
```

mixr\_models\_d.lib;JSBSim\_d.lib;libzmq\_d.lib;mixr\_ighost\_cigi\_d.lib;mixr\_ighost\_pov\_d.lib;ccl\_lib\_d.lib;
mixr\_interop\_dis\_d.lib;mixr\_interop\_d.lib;mixr\_simulation\_d.lib;mixr\_terrain\_d.lib;mixr\_ui\_glut\_d.lib;
mixr\_instruments\_d.lib;mixr\_graphics\_d.lib;mixr\_base\_d.lib;ftgl\_d.lib;freetype2\_d.lib;freeglut\_d.lib;
glu32.lib;opengl32.lib;Ws2\_32.lib;Winmm.lib;comctl32.lib;gdi32.lib;iphlpapi.lib;%(AdditionalDependencies)

````

### 4. **Visual Studio Configuration**

Inside the Visual Studio solution properties:

####   ➤ **Configuration Properties**
- Changed from `x64` to `Win32` under **Configuration Properties** to match the expected architecture of MIXR dependencies.

### 5. **Build Helper Scripts**

#### `make-edl.cmd` (inside the project folder)
Used to generate `.edl` files from `.epp` files:
```cmd
::
call ..\makedefs.cmd
%mcpp% display.epp > test0.edl %EPPFLAGS%
````

#### `makedefs.cmd` (copied from `mixr-examples`)

This script provides shared macro definitions and environment variable setups required by `make-edl.cmd`.

### 6. Export as Template

For convienent use, create a Visual Studio Template out of the project. 

At the top, in Visual Studio, go to: 

####   ➤ **Project > Export Template...**
 - Select Project template
 - Choose `templateProject` as the template to copy
 - You may change template name or add description; make any custom changes to the template
 - Press **Finish**

From here you may need to restart Visual Studio in order to see template. The template is useful but does not include the `file0.epp` or `make-edl.cmd` files. Those files need to be added manually (Sadly, until I find a way).

### 7. Set Up Complete

Now you may just go to the Solution Explorer, right-click and then **Add > New Project**. From there, find whatever you named the template from Step 6. It will create new project all set up.

---

## 📂 Current Projects

| Project Name | Description                                   |
| ------------ | --------------------------------------------- |
| demoShapes | Project demonstrating several basic shapes utilizing a display, pages, and graphics. |
| boidSimulation | Demonstration of a simple Boid simulation. Boids follow 3 rules to mimic flock behavior: Separation, Cohesion, and Alignment. |

---

```

No projects yet.
```
