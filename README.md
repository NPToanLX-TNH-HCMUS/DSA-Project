# 📚 DictforDev

> An advanced technical dictionary and roadmap visualizer designed for Software Engineering students. This project leverages optimized Data Structures and Algorithms to provide instant lookups and career guidance.

---

## 👥 Team 7 - 25C09
**Instructor:** Mr. Lê Trung Hoàng (University of Science - VNU-HCM)

| Student ID | Name | Role |
| --- | --- | --- |
| **25127522** | Ngô Phước Toàn | Leader - Data Specialist|
| **25127134** | Nguyễn Thành Tài | Searching Feature using Trie|
| **25127203** | Bùi Đăng Khôi | Graph Creating Feature using Graph Algorithms |

---

## 🌟 Key Features
- **Instant Search:** Lightning-fast lookup of developer-specific terminology.
- **Smart Autocomplete:** Prefix-based suggestions as you type.
- **Roadmap Visualization:** Explore prerequisites and learning paths for Competitive Programming and maybe more in the future.

---

## 🛠 Data Structures & Algorithms (Core DSA)

The heart of this project lies in the efficient implementation of specialized data structures:

### 1. Trie (Prefix Tree)
- **Use Case:** Implements the core search engine and autocomplete functionality.
- **Complexity:** $O(L)$ for search, insert, and delete (where $L$ is the length of the string). 
- **Efficiency:** Superior to Hash Tables for prefix-based operations, ensuring zero-latency suggestions.

### 2. Directed Graphs (Roadmaps)
- **Use Case:** Represents technology hierarchies and learning dependencies.
- **Algorithms:** - **BFS/DFS:** For traversing learning paths.
  - **Topological Sort:** (Optional/Planned) To determine the optimal order of learning technologies.

### 3. File I/O & Data Parsing
- Optimized loading mechanisms for handling large datasets stored in `.txt` and `.json` formats.

---

## 📊 Dataset
The dataset is curated by crawling professional career paths from **roadmap.sh** using Python scripts (`BeautifulSoup` & `requests`). 
- **Scale:** 10,000+ developer terms and 5+ comprehensive career roadmaps.
- **Format:** Pre-processed and normalized for C++ consumption.

---

## 🚀 Getting Started

### Prerequisites
- C++ Compiler

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
  g++ -o dictionary main.cpp 
  ```
4. Run the application:
  ```
  ./dictionary
  ```

---
## 📈 Experiment Results


---
## 🔮 Future Extensions
- **AI Integration:** Utilizing RAG (Retrieval-Augmented Generation) for more contextual definitions.

- **GUI Version:** Moving from CLI to a modern cross-platform Desktop GUI.

- **Cloud Sync:** Saving user progress across devices.

---
Created with ❤️ by Team 7 - 2026
