# Discrete_Structures
C++ implementation of Set Theory and Predicate Logic (∀, ∃, ∃!) with a custom DynamicArray, DiscreteSet, PredicateEngine, and graph algorithms (Kruskal's MST, Dijkstra's shortest path). Built from scratch using OOP, templates, and lambda functions. Discrete Mathematics project.

# ⚙️ Set Theory & Predicate Logic Engine in C++

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![OOP](https://img.shields.io/badge/Paradigm-OOP%20%2B%20Templates-green)
![Discrete Math](https://img.shields.io/badge/Subject-Discrete%20Mathematics-purple)
![University](https://img.shields.io/badge/FAST-CS%20Project-orange)

A C++ implementation of core Discrete Mathematics concepts — Set Theory, Predicate Logic with quantifiers, and Graph Algorithms — all built from scratch without relying on STL containers.

**Authors:** Muhammad Humd (24F-0542) & Muhammad Hammad Abid (24F-0612)
National University of Computing and Emerging Sciences (FAST) — Discrete Mathematics, 2024–25

---

## 📋 Overview

This project bridges mathematical theory and software engineering by implementing three layers:

1. A custom **DynamicArray** as the memory backbone
2. A **DiscreteSet + PredicateEngine** for set operations and logical quantifier evaluation
3. A **NetworkGraph** with Kruskal's MST and Dijkstra's Shortest Path algorithms

Everything is implemented using C++ templates and OOP principles — no `std::vector`, no `std::set`, no `std::priority_queue`.

---

## 🗂️ Repository Structure

```
Discrete-Math-Engine/
├── README.md
├── Discrete_Project.cpp     ← full source code (single file)
└── report/
    └── Project_Report.docx  ← submitted project report
```

---

## 🧩 Modules

### Module 1.1 — DynamicArray

A custom resizable array that serves as the underlying storage for all other modules.

| Feature | Detail |
|---|---|
| Initial capacity | 4 (doubles on overflow) |
| Insert | `pushBack()`, `pushAt(index)` |
| Delete | `deleteAt(index)` |
| Access | `operator[]` (read & write) |
| Memory | Manual heap allocation with destructor cleanup |

**Why not `std::vector`?**
Built manually to demonstrate memory management, amortized O(1) insertion, and resizing logic.

---

### Module 1.2 — DiscreteSet

A templated set class that enforces element uniqueness backed by `DynamicArray<T>`.

**Supported operations:**

| Operation | Symbol | Description |
|---|---|---|
| Union | A ∪ B | All elements in A or B |
| Intersection | A ∩ B | Elements in both A and B |
| Difference | A − B | Elements in A but not B |
| Subset check | C ⊆ A | Returns true/false |
| Contains | — | Membership test |
| Display | `<<` | Operator overloaded for printing |

**Sample output:**
```
A = {1, 2, 3, 4}
B = {3, 4, 5, 6}
C = {2, 4}

Union(A, B)        = {1, 2, 3, 4, 5, 6}
Intersection(A, B) = {3, 4}
Difference(A, B)   = {1, 2}
C ⊆ A              = True
```

---

### Module 1.3 — PredicateEngine

Evaluates logical predicates and quantifiers over sets using C++ lambda functions.

**Quantifiers supported:**

| Quantifier | Method | Meaning |
|---|---|---|
| ∀ x ∈ A: P(x) | `forall()` | True if ALL elements satisfy P |
| ∃ x ∈ A: P(x) | `exists()` | True if AT LEAST ONE element satisfies P |
| ∃! x ∈ A: P(x) | `exists_unique()` | True if EXACTLY ONE element satisfies P |
| ∀x ∃y: P(x,y) | `forall_exists()` | Nested quantifier (2 sets) |
| ∃x ∀y: P(x,y) | `exists_forall()` | Nested quantifier (2 sets) |

**Logical identity verification:**
```
¬∀x P(x)  ≡  ∃x ¬P(x)   → verified ✅
¬∃x P(x)  ≡  ∀x ¬P(x)   → verified ✅
```

**Sample output:**
```
∀ x ∈ A: even   → False
∃ x ∈ A: even   → True
∃! x ∈ A: even  → False
∀x ∈ A ∃y ∈ B: x < y  → True
∃x ∈ A ∀y ∈ B: x < y  → False
```

---

### Module 2.1 — NetworkGraph

A weighted undirected graph implemented using an adjacency matrix backed by `DynamicArray`.

- Add nodes and weighted edges
- Display adjacency matrix
- Serves as input for MST and shortest path algorithms

---

### Module 2.2 — Kruskal's MST

Finds the Minimum Spanning Tree of a weighted graph using the Union-Find (Disjoint Set Union) data structure.

```
Algorithm:
1. Sort all edges by weight
2. Pick smallest edge that does not form a cycle (Union-Find check)
3. Repeat until MST has (V-1) edges
```

| Property | Value |
|---|---|
| Time Complexity | O(E log E) |
| Space Complexity | O(V) |
| Data structure | Union-Find with path compression |

---

### Module 2.3 — Dijkstra's Shortest Path

Finds the shortest path between two nodes using a custom min-heap Priority Queue.

```
Algorithm:
1. Initialize distances to infinity, source = 0
2. Extract minimum distance node from heap
3. Relax all neighbor edges
4. Repeat until destination reached
```

| Property | Value |
|---|---|
| Time Complexity | O((V + E) log V) |
| Space Complexity | O(V) |
| Data structure | Custom binary min-heap |

---

### Module 3 — GraphConstraintBuilder (Integration)

Integrates set theory with graph theory. Verifies graph structural constraints using predicate logic — for example, checking whether all nodes satisfy a given connectivity property using `forall` or `exists` quantifiers.

---

## 🛠️ OOP Concepts Applied

| Concept | Where Used |
|---|---|
| **Encapsulation** | All classes use private data with controlled access |
| **Abstraction** | Set and PredicateEngine expose only mathematical interfaces |
| **Templates** | `DynamicArray<T>`, `DiscreteSet<T>` work with any data type |
| **Operator Overloading** | `operator[]` for array, `operator<<` for set display |
| **Polymorphism** | Lambda-based predicates allow flexible, generic logic |
| **RAII / Destructor** | DynamicArray frees heap memory in destructor |

---

## 🚀 How to Run

**Compile:**
```bash
g++ -std=c++17 -o project Discrete_Project.cpp
```

**Run:**
```bash
./project
```

**On Windows (MinGW):**
```bash
g++ -std=c++17 -o project.exe Discrete_Project.cpp
project.exe
```

---

## 📊 Complexity Summary

| Module | Operation | Time Complexity |
|---|---|---|
| DynamicArray | Insert (amortized) | O(1) |
| DynamicArray | Delete at index | O(n) |
| DiscreteSet | Insert / Contains | O(n) |
| DiscreteSet | Union / Intersection | O(n²) |
| PredicateEngine | forall / exists | O(n) |
| PredicateEngine | forall_exists | O(n²) |
| Kruskal's MST | Full algorithm | O(E log E) |
| Dijkstra's SP | Full algorithm | O((V+E) log V) |

---

## 📄 License

Submitted for academic purposes at FAST-NUCES. All code is original work by the authors.
