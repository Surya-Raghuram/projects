
#  Central Project Repository

## Overview
Welcome to my central project hub. This repository serves as a monorepo containing a collection of my tech projects, experiments, and tools. Each project is contained within its own dedicated directory with specific documentation.

##  Project List
Below is a list of the projects currently maintained in this repository:

| Project Name | Language / Tech Stack | Description |
| :--- | :--- | :--- |
| **[tempConvert](./tempConvert)** | C++, Nginx | A  temperature conversion service deployed using the Nginx web server. Demonstrates backend integration with C++. |
| **[filey](./filey)** | C++ | A custom binary file extension (`.filey`) implementation. Includes a writer to serialize data and a reader to verify and deserialize the custom format. |

## ⚠️ Important: How to Use
Since this is a central repository, **you cannot simply compile the root folder.** You must navigate into the specific project directory you wish to run.

If you wish to clone a project, you could do it in two ways: 
### 1. Clone the Repository and navigate to the project (Recommended): 
Clone the entire collection to your local machine:
```bash
git clone [https://github.com/Surya-Raghuram/projects.git](https://github.com/Surya-Raghuram/projects.git)
cd projects
```

### 2.Sparse Checkout the required folder:
Clone the entire repository without actually downloading the files other than the required directory using sparse checkout.

Given is an example to clone the `filey` directory
```
# 1. Clone the repo structure without downloading files (empty checkout)
git clone --no-checkout --depth 1 https://github.com/Surya-Raghuram/projects.git

# 2. Enter the repository folder
cd projects

# 3. Tell Git to enable "Sparse Checkout"
git sparse-checkout init --cone

# 4. Tell Git which folder you actually want
git sparse-checkout set filey

# 5. Download the files
git checkout main
```

#### Follow the instructions in the specific projects README

General Requirements
----
While each project has specific dependencies, most projects in this repo rely on:

    GCC/G++ Compiler (for C++ projects)

    Make 

    Nginx (specifically for tempConvert)

Maintained by Surya Raghuram