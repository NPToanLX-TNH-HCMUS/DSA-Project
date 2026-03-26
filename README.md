# 📚 DictForDevs (Team 7 - 25C09)
### *Advanced Technical Dictionary & Roadmap Visualizer for IT Students*

[![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)](https://isocpp.org/) [![NodeJS](https://img.shields.io/badge/Node.js-339933?style=flat&logo=nodedotjs&logoColor=white)](https://nodejs.org/) [![Status](https://img.shields.io/badge/Status-In--Development-orange)]()

![Logo](../DSA-Project/Image/Logo.png)
![Logo2](../DSA-Project/Image/Logo2.png)

> **DictForDevs** is a specialized technical dictionary and learning path visualizer tailored for Information Technology students. By leveraging optimized Data Structures and Algorithms (DSA), the platform provides near-instant terminology lookups and structured career guidance.

---

**Project's URL:** **https://dsa-project-1-b7gd.onrender.com/**

---

## 🧑‍💻 Team Members
**Instructor:** Mr. Lê Trung Hoàng (PhD, University of Science - VNU-HCM)

| Student ID | Name | Role | Primary Responsibility |
| :--- | :--- | :--- | :--- |
| **25127522** |Ngô Phước Toàn | Leader | Data Engineering and Frontend using ``CSS``|
| **25127134** | Nguyễn Thành Tài | Developer | Search Engine (Trie Implementation) and core Backend by ``NodeJS``|
| **25127203** | Bùi Đăng Khôi | Developer | Roadmap Retrival (Graph Algorithm) and Roadmap Visualization using ``ExpressJS``|

---

## 🌟 Key Features
- ⚡ **Instant Search:** High-speed lookup for over 600+ developer-centric terms with minimal latency.
- 🔍 **Smart Autocomplete:** Intuitive prefix-based suggestions provided as you type.
- 🗺️ **Roadmap Visualization:** Explore learning dependencies and prerequisites for **Competitive Programming** and will be more subjects in the future.
- 🌓 **Dev-Friendly UI:** Modern, minimalist interface with full **Light/Dark Mode** support.

---
## ⚙️ Tech Stack

* **Core Logic:** `C++` (Algorithm implementation and high-performance processing).
* **Backend:** `Node.js` & `Express.js` (Bridging the C++ engine with the web interface).
* **Data Crawling:** `Python` (`BeautifulSoup`, `Requests`) for automated data accumulating.
* **Deployment:** [Render](https://render.com/) Cloud Platform.

---

## 📊 Complete Dataset and Data Crawling Methodology 

We ensure a high-quality, comprehensive dataset to meet the specific needs of IT students. The data crawling proccess involves automated collection, cleaning, and restructured.

### 1. Data Sources:
We accumulated technical vocabularies industry-standard references:
* **[Awesome Developer Dictionary](https://github.com/ephraimduncan/awesome-developer-dictionary):** A GitHub-based repository for general dev terms.
* **[GeeksforGeeks](https://www.geeksforgeeks.org/computer-networks/technical-terminology-of-computer/):** Specialized computer networking and core CS terminology.
* **[VNOI Wiki](https://wiki.vnoi.info/):** High-level algorithms and data structures documentation.
* **[VNOI CP Roadmap](https://roadmap.sh/r/vnoi-roadmap):** The foundation for our manually designed [Competitive Programming Journey Roadmap](https://testcproadmap.my.canva.site/) - Core data for our Roadmap Creating feature.

### 2. Data Crawling:
We developed custom ``.py`` files located in the `Dataset_Pythonfiles` folder to automate data crawling from sources
* **Libraries:** `Requests` for HTTP handling and `BeautifulSoup` for HTML DOM objects parsing.
* **Process:** The scripts navigate through technical indices, extract definitions, and filter out irrelevant metadata.

### 3. Process:
To ensure compatibility between our C++ Core and JavaScript Backend, the raw data undergoes a transformation process:
* **Cleaning:** Removing HTML tags (using ``BeautifulSoup``), manually delete some "trash" data and normalizing special characters.
* **Formatting:** Data is converted from ``.txt`` files to ``.json`` files
* **Scale:** 600+ developer-specific terms and a comprehensive, multi-stage CP roadmap.
* **Optimization:** Data is pre-sorted or indexed to allow for $O(L)$ insertion into the Trie structure.

---


## 🕹️ Data Structures & Algorithms (Core DSA)

At the heart of **DictForDevs** is the manual implementation of specialized data structures designed for maximum efficiency:
### 1. Trie (Prefix Tree) — The Search Engine:
* **Purpose:** To power the **Instant Search** and **Smart Autocomplete** features.
* **Implementation Details:** 
    * Each node represents a character, allowing the system to traverse the tree as the user types.
    * **Asymptotic Complexity:** $O(L)$ for `Search`, `Insert`, and `Delete` operations (where $L$ is the length of the query string), making it independent of the dataset size ($N$).
* **Benefits:** By using a Trie, we achieve **zero-latency** suggestions, outperforming standard Hash Tables which would require $O(N)$ for prefix matching.

### 2. Directed Graphs — Roadmap Arrangement and Visualization:
* **Purpose:** To model the hierarchical dependencies of IT knowledge and the **Competitive Programming Journey**.
* **Key Algorithms:**
    * **BFS/DFS Traversal:** Used to dynamically render learning paths and identify related topics.
    * **Topological Sorting:** To provide a strictly ordered learning sequence for complex curriculum structures.

### 3. I/O Files Handle:
* **Data Parsing:** High-speed loading mechanisms to parse `.json` and `.txt` datasets into memory-resident structures at runtime by using ``json.hpp`` library.
* **C++ Integration:** Utilizing efficient memory allocation in C++ to handle 600+ nodes and their associated definitions.

---



## 🚀 Getting Started

> Main program is ``C++/main.cpp`` file, which is manually completed by our team members, and be used for GUI (Website) backend. Following below instructions to run our core C++ file 

### Prerequisites

- C++ Compiler
- Any IDE


### Installation

1. Clone the repository:

  ```

  git clone [https://github.com/NPToanLX-TNH-HCMUS/DSA-Project.git](https://github.com/NPToanLX-TNH-HCMUS/DSA-Project.git)

  ```

2. Navigate to the source folder:

  ```

  cd DSA-Project/C++

  ```

3. Compile the project:

  ```

  g++  main.cpp -o main.exe

  ```

4. Run the application:

  ```

  ./main

  ```



---

## 🔮 Future Extensions

- **Advanced DSA Integration:**
  - BFS: Suggestions for further learning
  - LCA (Lowest Common Ancestor): To find common foundational knowledge between two different topics.
  - Longest Path: To calculate the total difficulty or estimated time required for a specific roadmap.
  - Priority Queue: To rank the difficulty levels of algorithms within a specific category.

- **Ecosystem Expansion:** Developing a Chrome Extension for on-the-go lookups and building a Community-driven database where experts can contribute "battle-tested" roadmaps to bridge the gap between academia and industry.





---

*Created with ❤️ by **Team 7 with so much help from ChatGPT**, March, 2026*.
*Please give us some stars for fun 🌟🌟😁😁 !!*