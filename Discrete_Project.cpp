#include<iostream>
#include<utility>
#include<string>
#include<functional>
#include<climits>
#include<iomanip>
#include<algorithm>
#include<queue>

using namespace std;

// ==================== MODULE 1.1: DiscreteSet ====================

template<typename T>
class DynamicArray{
private:
    T* data;
    int size;
    int capacity;

public:
    DynamicArray(int initialCapacity = 4) {
        capacity = initialCapacity;
        size = 0;
        data = new T[capacity];
    }
    
    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        size = other.size;
        data = new T[capacity];
        for(int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    
    DynamicArray& operator=(const DynamicArray& other) {
        if(this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for(int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    int getSize()const{
        return size;
    }
    int getCapacity()const{
        return capacity;
    }
    T& operator[](int index){
        return data[index];
    }
    const T& operator[](int index)const{
        return data[index];
    }
    void resize(){
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for(int i=0;i<size;i++){
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    void pushAt(int index,const T& value){
        if(size == capacity){
            resize();
        }
        for(int i = size-1;i>=index;i--){
            data[i+1] = data[i];
        }
        data[index] = value;
        size++;
    }
    
    void deleteAt(int index){
        for(int i=index;i<size-1;i++){
            data[i] = data[i+1];
        }
        size--;
    }
    void pushBack(const T& value){
        pushAt(size, value);
    }
    void clear(){
        size = 0;
    }
    ~DynamicArray(){
        delete[] data;
    }
};

template<typename T>
class PairArray {
private:
    DynamicArray<pair<T, int>> data;
    
public:
    PairArray() {}
    
    int size() const {
        return data.getSize();
    }
    
    void push_back(const pair<T, int>& value) {
        data.pushBack(value);
    }
    
    pair<T, int>& operator[](int index) {
        return data[index];
    }
    
    const pair<T, int>& operator[](int index) const {
        return data[index];
    }
    
    void clear() {
        data.clear();
    }
};

template<typename T>
class DiscreteSet{
private:
    DynamicArray<T> elements;

    bool contains(const T& value) const {
        for(int i=0;i<elements.getSize();i++){
            if(elements[i]==value)
                return true;
        }
        return false;
    }

public:
    void insert(const T& value){
        if(!contains(value)){
            elements.pushBack(value);
        }
    }
    void remove(const T& value){
        for(int i=0;i<elements.getSize();i++){
            if(elements[i] == value){
                elements.deleteAt(i);
                break;
            }
        }
    }
    int size() const{
        return elements.getSize();
    }
    
    T& operator[](int index){
        return elements[index];
    }
    const T& operator[](int index) const{
        return elements[index];
    }
    
    void display() const {
        if (elements.getSize() == 0) {
            cout << "{}";
            return;
        }

        cout << "{";
        for (int i = 0; i < elements.getSize(); i++) {
            cout << elements[i];
            if (i != elements.getSize() - 1)
                cout << ", ";
        }
        cout << "}";
    }

    DiscreteSet<T> setUnion(const DiscreteSet<T>& other)const{
        DiscreteSet<T> result;
        for(int i =0;i<elements.getSize();i++){
            result.insert(elements[i]);
        }
        for(int i=0;i<other.elements.getSize();i++){
            result.insert(other.elements[i]);
        }
        return result;
    }
    
    DiscreteSet<T> setIntersection(const DiscreteSet<T>& other)const {
        DiscreteSet<T> result;
        for(int i=0;i<elements.getSize();i++){
            if(other.contains(elements[i])){
                result.insert(elements[i]);
            }
        }
        return result;
    }
    
    DiscreteSet<T> setDifference(const DiscreteSet<T>& other)const{
        DiscreteSet<T> result;
        for(int i=0;i<elements.getSize();i++){
            if(!other.contains(elements[i])){
                result.insert(elements[i]);
            }
        }
        return result;
    }

    bool isSubsetOf(const DiscreteSet<T>& other)const {
        for(int i=0;i<elements.getSize();i++){
            if(!other.contains(elements[i])){
                return false;
            }
        }
        return true;
    }
};

// ==================== MODULE 1.2: PredicateEngine ====================

template<typename T>
using LogicalPredicate = function<bool(const T&)>;

template<typename T>
class PredicateEngine {
public:
    bool forall(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        for(int i = 0; i < universe.size(); i++) {
            if(!P(universe[i])) {
                return false;
            }
        }
        return true;
    }
    
    bool exists(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        for(int i = 0; i < universe.size(); i++) {
            if(P(universe[i])) {
                return true;
            }
        }
        return false;
    }
    
    bool exists_unique(const DiscreteSet<T>& universe, LogicalPredicate<T> P) {
        int count = 0;
        for(int i = 0; i < universe.size(); i++) {
            if(P(universe[i])) {
                count++;
                if(count > 1) {
                    return false;
                }
            }
        }
        return count == 1;
    }
    
    template<typename U>
    bool forall_exists(const DiscreteSet<T>& universe1, 
                       const DiscreteSet<U>& universe2,
                       function<bool(const T&, const U&)> P) {
        for(int i = 0; i < universe1.size(); i++) {
            bool found = false;
            for(int j = 0; j < universe2.size(); j++) {
                if(P(universe1[i], universe2[j])) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                return false;
            }
        }
        return true;
    }
    
    template<typename U>
    bool exists_forall(const DiscreteSet<T>& universe1,
                       const DiscreteSet<U>& universe2,
                       function<bool(const T&, const U&)> P) {
        for(int i = 0; i < universe1.size(); i++) {
            bool all_satisfy = true;
            for(int j = 0; j < universe2.size(); j++) {
                if(!P(universe1[i], universe2[j])) {
                    all_satisfy = false;
                    break;
                }
            }
            if(all_satisfy) {
                return true;
            }
        }
        return false;
    }
    
    bool verify_not_forall_equals_exists_not(const DiscreteSet<T>& universe,
                                             LogicalPredicate<T> P) {
        bool not_forall = !forall(universe, P);
        LogicalPredicate<T> not_P = [P](const T& x) { return !P(x); };
        bool exists_not = exists(universe, not_P);
        return not_forall == exists_not;
    }
    
    bool verify_not_exists_equals_forall_not(const DiscreteSet<T>& universe,
                                             LogicalPredicate<T> P) {
        bool not_exists = !exists(universe, P);
        LogicalPredicate<T> not_P = [P](const T& x) { return !P(x); };
        bool forall_not = forall(universe, not_P);
        return not_exists == forall_not;
    }
};

// ==================== MODULE 1.3: GraphConstraintBuilder ====================

template<typename T>
class GraphConstraintBuilder {
private:
    DiscreteSet<T> universe;
    
public:
    void set_universe(const DiscreteSet<T>& nodes) {
        universe = nodes;
    }
    
    DiscreteSet<T> compute_mandatory_nodes(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic,
        const DiscreteSet<T>& redundancy_nodes
    ) const {
        DiscreteSet<T> union_set = critical_infrastructure.setUnion(high_traffic);
        DiscreteSet<T> mandatory = union_set.setIntersection(redundancy_nodes);
        return mandatory;
    }
    
    DiscreteSet<T> compute_excluded_nodes(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic,
        const DiscreteSet<T>& redundancy_nodes
    ) const {
        DiscreteSet<T> temp = critical_infrastructure.setUnion(high_traffic);
        DiscreteSet<T> important_nodes = temp.setUnion(redundancy_nodes);
        DiscreteSet<T> excluded = universe.setDifference(important_nodes);
        return excluded;
    }
    
    DiscreteSet<T> compute_optional_nodes(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic,
        const DiscreteSet<T>& redundancy_nodes
    ) const {
        DiscreteSet<T> mandatory = compute_mandatory_nodes(
            critical_infrastructure, high_traffic, redundancy_nodes
        );
        DiscreteSet<T> temp = critical_infrastructure.setUnion(high_traffic);
        DiscreteSet<T> all_important = temp.setUnion(redundancy_nodes);
        DiscreteSet<T> optional = all_important.setDifference(mandatory);
        return optional;
    }
    
    DiscreteSet<T> compute_critical_path_nodes(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic
    ) const {
        return critical_infrastructure.setIntersection(high_traffic);
    }
    
    DiscreteSet<T> compute_redundancy_only_nodes(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic,
        const DiscreteSet<T>& redundancy_nodes
    ) const {
        DiscreteSet<T> union_set = critical_infrastructure.setUnion(high_traffic);
        return redundancy_nodes.setDifference(union_set);
    }
    
    bool verify_constraints(
        const DiscreteSet<T>& critical_infrastructure,
        const DiscreteSet<T>& high_traffic,
        const DiscreteSet<T>& redundancy_nodes
    ) const {
        DiscreteSet<T> mandatory = compute_mandatory_nodes(
            critical_infrastructure, high_traffic, redundancy_nodes
        );
        DiscreteSet<T> excluded = compute_excluded_nodes(
            critical_infrastructure, high_traffic, redundancy_nodes
        );
        
        DiscreteSet<T> overlap = mandatory.setIntersection(excluded);
        
        if (overlap.size() > 0) {
            return false;
        }
        
        if (!mandatory.isSubsetOf(universe)) {
            return false;
        }
        if (!excluded.isSubsetOf(universe)) {
            return false;
        }
        
        return true;
    }
};

// ==================== MODULE 2.1: NetworkGraph ====================

template<typename T>
class NetworkGraph {
private:
    int num_vertices;
    DynamicArray<DynamicArray<int>> adjacency_matrix;
    DynamicArray<PairArray<T>> adjacency_list;
    DynamicArray<T> vertex_labels;
    
    int get_vertex_index(const T& vertex) const {
        for(int i = 0; i < vertex_labels.getSize(); i++) {
            if(vertex_labels[i] == vertex) {
                return i;
            }
        }
        return -1;
    }
    
public:
    NetworkGraph(const DiscreteSet<T>& vertices) {
        num_vertices = vertices.size();
        
        // Store vertex labels
        for(int i = 0; i < num_vertices; i++) {
            vertex_labels.pushBack(vertices[i]);
        }
        
        // Initialize adjacency matrix with zeros
        for(int i = 0; i < num_vertices; i++) {
            DynamicArray<int> row(num_vertices);
            for(int j = 0; j < num_vertices; j++) {
                row.pushBack(0);
            }
            adjacency_matrix.pushBack(row);
        }
        
        // Initialize adjacency list
        for(int i = 0; i < num_vertices; i++) {
            PairArray<T> empty_list;
            adjacency_list.pushBack(empty_list);
        }
    }
    
    void add_edge(const T& u, const T& v, int weight) {
        int u_idx = get_vertex_index(u);
        int v_idx = get_vertex_index(v);
        
        if (u_idx == -1 || v_idx == -1 || weight <= 0) {
            return;
        }
        adjacency_matrix[u_idx][v_idx] = weight;
        adjacency_matrix[v_idx][u_idx] = weight;
        adjacency_list[u_idx].push_back({v, weight});
        adjacency_list[v_idx].push_back({u, weight});
    }
    
    int get_num_vertices() const {
        return num_vertices;
    }
    
    T get_vertex_label(int index) const {
        if(index >= 0 && index < vertex_labels.getSize()) {
            return vertex_labels[index];
        }
        return T();
    }
    
    const DynamicArray<DynamicArray<int>>& get_adjacency_matrix() const {
        return adjacency_matrix;
    }
    
    const DynamicArray<PairArray<T>>& get_adjacency_list() const {
        return adjacency_list;
    }
    
    void display_matrix() const {
        cout << "\nAdjacency Matrix:\n    ";
        for (int i = 0; i < num_vertices; i++) {
            cout << setw(4) << vertex_labels[i];
        }
        cout << "\n";
        for (int i = 0; i < num_vertices; i++) {
            cout << setw(4) << vertex_labels[i] << " ";
            for (int j = 0; j < num_vertices; j++) {
                if (adjacency_matrix[i][j] == 0)
                    cout << setw(4) << "-";
                else
                    cout << setw(4) << adjacency_matrix[i][j];
            }
            cout << "\n";
        }
    }
};

// ==================== MODULE 2.2: Kruskal's MST ====================

template<typename T>
struct Edge {
    T u, v;
    int weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

template<typename T>
class UnionFind {
private:
    DynamicArray<int> parent;
    DynamicArray<int> rank;
    
public:
    UnionFind(int n) {
        for (int i = 0; i < n; i++) {
            parent.pushBack(i);
            rank.pushBack(0);
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) {
            return false;
        }
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

template<typename T>
struct MSTResult {
    bool success;
    DynamicArray<Edge<T>> mst_edges;
    int total_weight;
    DiscreteSet<T> included_vertices;
};

template<typename T>
MSTResult<T> kruskals_mst(const NetworkGraph<T>& graph, const T& root_vertex, const DiscreteSet<T>& mandatory_vertices) {
    MSTResult<T> result;
    result.success = false;
    result.total_weight = 0;
    
    int n = graph.get_num_vertices();
    const auto& matrix = graph.get_adjacency_matrix();
    
    DynamicArray<Edge<T>> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] > 0) {
                edges.pushBack({graph.get_vertex_label(i), graph.get_vertex_label(j), matrix[i][j]});
            }
        }
    }
    
    // Sort edges using bubble sort
    for (int i = 0; i < edges.getSize() - 1; i++) {
        for (int j = 0; j < edges.getSize() - i - 1; j++) {
            if (edges[j+1] < edges[j]) {
                Edge<T> temp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = temp;
            }
        }
    }
    
    UnionFind<T> uf(n);
    DynamicArray<bool> in_mst(n);
    for (int i = 0; i < n; i++) {
        in_mst.pushBack(false);
    }
    
    for (int k = 0; k < edges.getSize(); k++) {
        const Edge<T>& edge = edges[k];
        // Get indices for union-find
        int u_idx = -1, v_idx = -1;
        for(int i = 0; i < n; i++) {
            if(graph.get_vertex_label(i) == edge.u) u_idx = i;
            if(graph.get_vertex_label(i) == edge.v) v_idx = i;
        }
        
        if(u_idx != -1 && v_idx != -1 && uf.unite(u_idx, v_idx)) {
            result.mst_edges.pushBack(edge);
            result.total_weight += edge.weight;
            in_mst[u_idx] = true;
            in_mst[v_idx] = true;
            
            if (result.mst_edges.getSize() == n - 1) {
                break;
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (in_mst[i]) {
            result.included_vertices.insert(graph.get_vertex_label(i));
        }
    }
    
    bool all_mandatory_included = true;
    for (int i = 0; i < mandatory_vertices.size(); i++) {
        bool found = false;
        for(int j = 0; j < n; j++) {
            if(in_mst[j] && graph.get_vertex_label(j) == mandatory_vertices[i]) {
                found = true;
                break;
            }
        }
        if(!found) {
            all_mandatory_included = false;
            break;
        }
    }
    
    result.success = (result.mst_edges.getSize() == n - 1) && all_mandatory_included;
    
    return result;
}

// ==================== MODULE 2.3: Dijkstra's Shortest Path ====================

template<typename T>
struct DijkstraResult {
    bool path_exists;
    DynamicArray<T> path;
    int total_distance;
};

// Custom Priority Queue implementation (min-heap)
template<typename T>
class PriorityQueue {
private:
    DynamicArray<T> heap;
    function<bool(const T&, const T&)> compare;
    
    void heapify_up(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (compare(heap[index], heap[parent])) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }
    
    void heapify_down(int index) {
        int size = heap.getSize();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;
            
            if (left < size && compare(heap[left], heap[smallest])) {
                smallest = left;
            }
            if (right < size && compare(heap[right], heap[smallest])) {
                smallest = right;
            }
            
            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }
    
public:
    PriorityQueue(function<bool(const T&, const T&)> comp = less<T>()) : compare(comp) {}
    
    void push(const T& value) {
        heap.pushBack(value);
        heapify_up(heap.getSize() - 1);
    }
    
    void pop() {
        if (empty()) return;
        heap[0] = heap[heap.getSize() - 1];
        heap.deleteAt(heap.getSize() - 1);
        if (!empty()) {
            heapify_down(0);
        }
    }
    
    T top() const {
        return heap[0];
    }
    
    bool empty() const {
        return heap.getSize() == 0;
    }
    
    int size() const {
        return heap.getSize();
    }
};

template<typename T>
DijkstraResult<T> dijkstra_shortest_path(const NetworkGraph<T>& graph, const T& source, const T& destination) {
    DijkstraResult<T> result;
    result.path_exists = false;
    result.total_distance = INT_MAX;
    
    int n = graph.get_num_vertices();
    const auto& adj_list = graph.get_adjacency_list();
    
    // Find source and destination indices
    int source_idx = -1, dest_idx = -1;
    for(int i = 0; i < n; i++) {
        if(graph.get_vertex_label(i) == source) source_idx = i;
        if(graph.get_vertex_label(i) == destination) dest_idx = i;
    }
    
    if(source_idx == -1 || dest_idx == -1) {
        return result;
    }
    
    DynamicArray<int> dist(n);
    DynamicArray<int> parent(n);
    DynamicArray<bool> visited(n);
    
    for (int i = 0; i < n; i++) {
        dist.pushBack(INT_MAX);
        parent.pushBack(-1);
        visited.pushBack(false);
    }
    
    PriorityQueue<pair<int, int>> pq([](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first < b.first;
    });
    
    dist[source_idx] = 0;
    pq.push({0, source_idx});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        if (u == dest_idx) break;
        
        for (int i = 0; i < adj_list[u].size(); i++) {
            // Find neighbor index
            int v_idx = -1;
            for(int j = 0; j < n; j++) {
                if(graph.get_vertex_label(j) == adj_list[u][i].first) {
                    v_idx = j;
                    break;
                }
            }
            
            if(v_idx != -1 && !visited[v_idx] && dist[u] + adj_list[u][i].second < dist[v_idx]) {
                dist[v_idx] = dist[u] + adj_list[u][i].second;
                parent[v_idx] = u;
                pq.push({dist[v_idx], v_idx});
            }
        }
    }
    
    if (dist[dest_idx] != INT_MAX) {
        result.path_exists = true;
        result.total_distance = dist[dest_idx];
        
        int current = dest_idx;
        while (current != -1) {
            result.path.pushAt(0, graph.get_vertex_label(current));
            current = parent[current];
        }
    }
    
    return result;
}

// ==================== MODULE 3: ADVANCED INTEGRATION MODULE ====================

template<typename T>
class AdvancedIntegrationModule {
private:
    NetworkGraph<T>* current_graph;
    DiscreteSet<T> current_universe;
    GraphConstraintBuilder<T> constraint_builder;
    
public:
    AdvancedIntegrationModule() : current_graph(nullptr) {}
    
    void set_current_graph(NetworkGraph<T>* graph) {
        current_graph = graph;
    }
    
    void interactive_universe_setup() {
        cout << "\n=== SET UP UNIVERSE FOR CONSTRAINT ANALYSIS ===\n";
        
        int n;
        T value;
        cout << "Enter number of nodes in universe: ";
        cin >> n;
        cout << "Enter node values (can be numbers or characters): ";
        for(int i = 0; i < n; i++) {
            cin >> value;
            current_universe.insert(value);
        }
        
        constraint_builder.set_universe(current_universe);
        cout << "Universe set: "; current_universe.display(); cout << endl;
    }
    
    void interactive_constraint_analysis() {
        if (current_universe.size() == 0) {
            cout << "Please set up universe first!\n";
            return;
        }
        
        cout << "\n=== INTERACTIVE CONSTRAINT ANALYSIS ===\n";
        
        DiscreteSet<T> critical, high_traffic, redundancy;
        int n;
        T value;
        
        // Get critical infrastructure nodes
        cout << "\nEnter number of critical infrastructure nodes: ";
        cin >> n;
        if (n > 0) {
            cout << "Enter critical nodes: ";
            for(int i = 0; i < n; i++) {
                cin >> value;
                critical.insert(value);
            }
        }
        
        // Get high traffic nodes
        cout << "\nEnter number of high traffic nodes: ";
        cin >> n;
        if (n > 0) {
            cout << "Enter high traffic nodes: ";
            for(int i = 0; i < n; i++) {
                cin >> value;
                high_traffic.insert(value);
            }
        }
        
        // Get redundancy nodes
        cout << "\nEnter number of redundancy nodes: ";
        cin >> n;
        if (n > 0) {
            cout << "Enter redundancy nodes: ";
            for(int i = 0; i < n; i++) {
                cin >> value;
                redundancy.insert(value);
            }
        }
        
        // Display results
        cout << "\n=== CONSTRAINT ANALYSIS RESULTS ===\n";
        cout << "Universe: "; current_universe.display(); cout << endl;
        cout << "Critical Infrastructure: "; critical.display(); cout << endl;
        cout << "High Traffic: "; high_traffic.display(); cout << endl;
        cout << "Redundancy: "; redundancy.display(); cout << endl;
        
        auto mandatory = constraint_builder.compute_mandatory_nodes(critical, high_traffic, redundancy);
        auto excluded = constraint_builder.compute_excluded_nodes(critical, high_traffic, redundancy);
        auto optional = constraint_builder.compute_optional_nodes(critical, high_traffic, redundancy);
        auto critical_path = constraint_builder.compute_critical_path_nodes(critical, high_traffic);
        auto redundancy_only = constraint_builder.compute_redundancy_only_nodes(critical, high_traffic, redundancy);
        
        cout << "\nMandatory Nodes: "; mandatory.display(); cout << endl;
        cout << "Excluded Nodes: "; excluded.display(); cout << endl;
        cout << "Optional Nodes: "; optional.display(); cout << endl;
        cout << "Critical Path Nodes: "; critical_path.display(); cout << endl;
        cout << "Redundancy-Only Nodes: "; redundancy_only.display(); cout << endl;
        
        bool valid = constraint_builder.verify_constraints(critical, high_traffic, redundancy);
        cout << "\nConstraints are " << (valid ? "VALID ✓" : "INVALID ✗") << endl;
    }
    
    void integrated_graph_analysis() {
        if (!current_graph) {
            cout << "Please create a graph first!\n";
            return;
        }
        
        if (current_universe.size() == 0) {
            cout << "Please set up universe first!\n";
            return;
        }
        
        cout << "\n=== INTEGRATED GRAPH ANALYSIS ===\n";
        
        // Get constraint sets from user
        DiscreteSet<T> critical, high_traffic, redundancy;
        int n;
        T value;
        
        cout << "\n=== Enter Constraint Sets ===\n";
        
        cout << "Enter number of critical infrastructure nodes: ";
        cin >> n;
        cout << "Enter critical nodes: ";
        for(int i = 0; i < n; i++) {
            cin >> value;
            critical.insert(value);
        }
        
        cout << "\nEnter number of high traffic nodes: ";
        cin >> n;
        cout << "Enter high traffic nodes: ";
        for(int i = 0; i < n; i++) {
            cin >> value;
            high_traffic.insert(value);
        }
        
        cout << "\nEnter number of redundancy nodes: ";
        cin >> n;
        cout << "Enter redundancy nodes: ";
        for(int i = 0; i < n; i++) {
            cin >> value;
            redundancy.insert(value);
        }
        
        // Compute mandatory nodes
        auto mandatory = constraint_builder.compute_mandatory_nodes(critical, high_traffic, redundancy);
        
        cout << "\n=== Running Kruskal's MST with Mandatory Nodes ===\n";
        cout << "Mandatory nodes: "; mandatory.display(); cout << endl;
        
        T root = mandatory.size() > 0 ? mandatory[0] : T();
        MSTResult<T> mst_result = kruskals_mst(*current_graph, root, mandatory);
        
        cout << "\nMST Results:\n";
        cout << "Success: " << (mst_result.success ? "YES ✓" : "NO ✗") << endl;
        cout << "Total Weight: " << mst_result.total_weight << endl;
        cout << "Edges in MST:\n";
        for(int i = 0; i < mst_result.mst_edges.getSize(); i++) {
            const auto& edge = mst_result.mst_edges[i];
            cout << "  (" << edge.u << " -- " << edge.v << ") weight: " << edge.weight << endl;
        }
        
        // Shortest path analysis between mandatory nodes
        if (mandatory.size() >= 2) {
            cout << "\n=== Shortest Path Analysis Between Mandatory Nodes ===\n";
            T source = mandatory[0];
            T dest = mandatory[mandatory.size() > 1 ? 1 : 0];
            
            cout << "Finding shortest path from node " << source << " to node " << dest << endl;
            DijkstraResult<T> dijkstra_result = dijkstra_shortest_path(*current_graph, source, dest);
            
            if (dijkstra_result.path_exists) {
                cout << "Shortest path distance: " << dijkstra_result.total_distance << endl;
                cout << "Path: ";
                for(int i = 0; i < dijkstra_result.path.getSize(); i++) {
                    cout << dijkstra_result.path[i];
                    if (i < dijkstra_result.path.getSize() - 1) cout << " → ";
                }
                cout << endl;
            } else {
                cout << "No path exists between these mandatory nodes!\n";
            }
        }
    }
    
    void comprehensive_test_suite() {
        cout << "\n=== COMPREHENSIVE TEST SUITE ===\n";
        
        // Test 1: Set operations
        cout << "\n--- Test 1: Set Operations ---\n";
        DiscreteSet<T> setA, setB;
        setA.insert('A'); setA.insert('B'); setA.insert('C');
        setB.insert('C'); setB.insert('D'); setB.insert('E');
        
        cout << "A = "; setA.display(); cout << endl;
        cout << "B = "; setB.display(); cout << endl;
        cout << "A ∪ B = "; setA.setUnion(setB).display(); cout << endl;
        cout << "A ∩ B = "; setA.setIntersection(setB).display(); cout << endl;
        
        // Test 2: Predicate logic
        cout << "\n--- Test 2: Predicate Logic ---\n";
        PredicateEngine<T> predEngine;
        LogicalPredicate<T> isVowel = [](T x) { 
            return x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U'; 
        };
        
        DiscreteSet<T> testSet;
        testSet.insert('A'); testSet.insert('B'); testSet.insert('C'); 
        testSet.insert('D'); testSet.insert('E');
        
        cout << "Set = "; testSet.display(); cout << endl;
        cout << "All vowels? " << (predEngine.forall(testSet, isVowel) ? "Yes" : "No") << endl;
        cout << "Exists vowel? " << (predEngine.exists(testSet, isVowel) ? "Yes" : "No") << endl;
        
        // Test 3: Graph analysis (if graph exists)
        if (current_graph) {
            cout << "\n--- Test 3: Graph Analysis ---\n";
            current_graph->display_matrix();
        }
        
        cout << "\n=== TEST SUITE COMPLETE ===\n";
    }
};

// ==================== MODULE 1 TEMPLATE FUNCTIONS ====================

template<typename T>
void testDiscreteSet() {
    cout << "\n=== DISCRETE SET OPERATIONS ===\n";
    
    DiscreteSet<T> A, B;
    int n;
    T value;
    
    cout << "\nEnter number of elements for Set A: ";
    cin >> n;
    cout << "Enter elements (can be numbers or characters): ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        A.insert(value);
    }
    
    cout << "\nEnter number of elements for Set B: ";
    cin >> n;
    cout << "Enter elements: ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        B.insert(value);
    }
    
    cout << "\n[Results]\n";
    cout << "Set A: "; A.display(); cout << "\n";
    cout << "Set B: "; B.display(); cout << "\n";
    cout << "Union (A ∪ B): "; A.setUnion(B).display(); cout << "\n";
    cout << "Intersection (A ∩ B): "; A.setIntersection(B).display(); cout << "\n";
    cout << "Difference (A \\ B): "; A.setDifference(B).display(); cout << "\n";
    cout << "Is A subset of B? " << (A.isSubsetOf(B) ? "Yes" : "No") << "\n";
}

template<typename T>
void testPredicateEngine() {
    cout << "\n=== PREDICATE ENGINE ===\n";
    
    DiscreteSet<T> testSet;
    int n;
    T value;
    
    cout << "\nEnter number of elements: ";
    cin >> n;
    cout << "Enter elements (can be numbers or characters): ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        testSet.insert(value);
    }
    
    cout << "\nTest Set: "; testSet.display(); cout << "\n";
    
    PredicateEngine<T> engine;
    
    // Type-specific predicates
    if constexpr (is_same_v<T, int>) {
        // Integer predicates
        LogicalPredicate<T> isEven = [](const T& x) { return x % 2 == 0; };
        LogicalPredicate<T> greaterThan5 = [](const T& x) { return x > 5; };
        LogicalPredicate<T> isPrime = [](const T& x) {
            if (x <= 1) return false;
            for (int i = 2; i * i <= x; i++) {
                if (x % i == 0) return false;
            }
            return true;
        };
        
        cout << "\n[Test Results - Integer Predicates]\n";
        cout << "All elements even? " << (engine.forall(testSet, isEven) ? "Yes" : "No") << "\n";
        cout << "Exists element > 5? " << (engine.exists(testSet, greaterThan5) ? "Yes" : "No") << "\n";
        cout << "Exists prime number? " << (engine.exists(testSet, isPrime) ? "Yes" : "No") << "\n";
        cout << "Exists unique even number? " << (engine.exists_unique(testSet, isEven) ? "Yes" : "No") << "\n";
        
        // Test quantifier equivalences
        cout << "\n[Quantifier Logic Tests]\n";
        cout << "¬∀x P(x) ≡ ∃x ¬P(x) where P(x): x is even: ";
        cout << (engine.verify_not_forall_equals_exists_not(testSet, isEven) ? "VALID ✓" : "INVALID ✗") << "\n";
        cout << "¬∃x P(x) ≡ ∀x ¬P(x) where P(x): x is even: ";
        cout << (engine.verify_not_exists_equals_forall_not(testSet, isEven) ? "VALID ✓" : "INVALID ✗") << "\n";
        
    } else if constexpr (is_same_v<T, char>) {
        // Character predicates
        LogicalPredicate<T> isVowel = [](const T& x) { 
            char c = toupper(x);
            return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'; 
        };
        LogicalPredicate<T> isUppercase = [](const T& x) { return x >= 'A' && x <= 'Z'; };
        LogicalPredicate<T> isConsonant = [](const T& x) { 
            char c = toupper(x);
            return (c >= 'A' && c <= 'Z') && !(c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
        };
        
        cout << "\n[Test Results - Character Predicates]\n";
        cout << "All elements vowels? " << (engine.forall(testSet, isVowel) ? "Yes" : "No") << "\n";
        cout << "Exists uppercase letter? " << (engine.exists(testSet, isUppercase) ? "Yes" : "No") << "\n";
        cout << "Exists consonant? " << (engine.exists(testSet, isConsonant) ? "Yes" : "No") << "\n";
        cout << "Exists unique vowel? " << (engine.exists_unique(testSet, isVowel) ? "Yes" : "No") << "\n";
        
        // Test quantifier equivalences
        cout << "\n[Quantifier Logic Tests]\n";
        cout << "¬∀x P(x) ≡ ∃x ¬P(x) where P(x): x is vowel: ";
        cout << (engine.verify_not_forall_equals_exists_not(testSet, isVowel) ? "VALID ✓" : "INVALID ✗") << "\n";
        cout << "¬∃x P(x) ≡ ∀x ¬P(x) where P(x): x is vowel: ";
        cout << (engine.verify_not_exists_equals_forall_not(testSet, isVowel) ? "VALID ✓" : "INVALID ✗") << "\n";
        
    } else {
        // Generic fallback predicate
        LogicalPredicate<T> isDefault = [](const T& x) { return true; };
        cout << "\n[Generic Test Results]\n";
        cout << "All elements satisfy true? " << (engine.forall(testSet, isDefault) ? "Yes" : "No") << "\n";
    }
}

template<typename T>
void testGraphConstraintBuilder() {
    cout << "\n=== GRAPH CONSTRAINT BUILDER ===\n";
    
    DiscreteSet<T> critical, high_traffic, redundancy;
    int n;
    T value;
    
    cout << "\n=== SET UP UNIVERSE ===\n";
    DiscreteSet<T> universe;
    cout << "Enter number of nodes in universe: ";
    cin >> n;
    cout << "Enter universe nodes (can be numbers or characters): ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        universe.insert(value);
    }
    
    cout << "\n=== ENTER CONSTRAINT SETS ===\n";
    
    cout << "Enter number of critical nodes: ";
    cin >> n;
    cout << "Enter critical nodes: ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        critical.insert(value);
    }
    
    cout << "\nEnter number of high traffic nodes: ";
    cin >> n;
    cout << "Enter high traffic nodes: ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        high_traffic.insert(value);
    }
    
    cout << "\nEnter number of redundancy nodes: ";
    cin >> n;
    cout << "Enter redundancy nodes: ";
    for(int i = 0; i < n; i++) {
        cin >> value;
        redundancy.insert(value);
    }
    
    GraphConstraintBuilder<T> builder;
    builder.set_universe(universe);
    
    cout << "\n=== CONSTRAINT ANALYSIS RESULTS ===\n";
    cout << "Universe: "; universe.display(); cout << "\n";
    cout << "Critical Nodes: "; critical.display(); cout << "\n";
    cout << "High Traffic Nodes: "; high_traffic.display(); cout << "\n";
    cout << "Redundancy Nodes: "; redundancy.display(); cout << "\n";
    
    auto mandatory = builder.compute_mandatory_nodes(critical, high_traffic, redundancy);
    auto excluded = builder.compute_excluded_nodes(critical, high_traffic, redundancy);
    auto optional = builder.compute_optional_nodes(critical, high_traffic, redundancy);
    auto critical_path = builder.compute_critical_path_nodes(critical, high_traffic);
    auto redundancy_only = builder.compute_redundancy_only_nodes(critical, high_traffic, redundancy);
    
    cout << "\nMandatory Nodes: "; mandatory.display(); cout << "\n";
    cout << "Excluded Nodes: "; excluded.display(); cout << "\n";
    cout << "Optional Nodes: "; optional.display(); cout << "\n";
    cout << "Critical Path Nodes: "; critical_path.display(); cout << "\n";
    cout << "Redundancy-Only Nodes: "; redundancy_only.display(); cout << "\n";
    
    bool valid = builder.verify_constraints(critical, high_traffic, redundancy);
    cout << "\nConstraints are " << (valid ? "VALID ✓" : "INVALID ✗") << "\n";
}

// ==================== MODULE 2 FUNCTIONS ====================

template<typename T>
NetworkGraph<T>* createGraph() {
    int vertices, edges;
    cout << "\nEnter number of vertices: ";
    cin >> vertices;
    
    // Get vertex labels
    DiscreteSet<T> vertex_set;
    cout << "Enter vertex labels (can be numbers or characters): ";
    for(int i = 0; i < vertices; i++) {
        T label;
        cin >> label;
        vertex_set.insert(label);
    }
    
    NetworkGraph<T>* graph = new NetworkGraph<T>(vertex_set);
    
    cout << "Enter number of edges: ";
    cin >> edges;
    
    cout << "Enter edges (vertex1 vertex2 weight):\n";
    for(int i = 0; i < edges; i++) {
        T u, v;
        int w;
        cin >> u >> v >> w;
        graph->add_edge(u, v, w);
    }
    
    graph->display_matrix();
    return graph;
}

template<typename T>
void runKruskalMST(NetworkGraph<T>* graph) {
    if(!graph) {
        cout << "Please create a graph first!\n";
        return;
    }
    
    DiscreteSet<T> mandatory;
    int n;
    T value;
    
    cout << "\nEnter number of mandatory vertices: ";
    cin >> n;
    if(n > 0) {
        cout << "Enter vertices: ";
        for(int i = 0; i < n; i++) {
            cin >> value;
            mandatory.insert(value);
        }
    }
    
    T root = mandatory.size() > 0 ? mandatory[0] : T();
    MSTResult<T> result = kruskals_mst(*graph, root, mandatory);
    
    cout << "\n[Kruskal's MST Results]\n";
    cout << "Success: " << (result.success ? "YES ✓" : "NO ✗") << "\n";
    cout << "Total Weight: " << result.total_weight << "\n";
    cout << "Edges:\n";
    for(int i = 0; i < result.mst_edges.getSize(); i++) {
        const auto& edge = result.mst_edges[i];
        cout << "  (" << edge.u << " -- " << edge.v << ") weight: " << edge.weight << "\n";
    }
}

template<typename T>
void runDijkstraPath(NetworkGraph<T>* graph) {
    if(!graph) {
        cout << "Please create a graph first!\n";
        return;
    }
    
    T source, dest;
    cout << "\nEnter source vertex: ";
    cin >> source;
    cout << "Enter destination vertex: ";
    cin >> dest;
    
    DijkstraResult<T> result = dijkstra_shortest_path(*graph, source, dest);
    
    cout << "\n[Dijkstra's Shortest Path Results]\n";
    if(result.path_exists) {
        cout << "Path exists: YES ✓\n";
        cout << "Distance: " << result.total_distance << "\n";
        cout << "Path: ";
        for(int i = 0; i < result.path.getSize(); i++) {
            cout << result.path[i];
            if(i < result.path.getSize() - 1) cout << " → ";
        }
        cout << "\n";
    } else {
        cout << "No path exists!\n";
    }
}

// ==================== MENU FUNCTIONS ====================

void displayMainMenu() {
   cout<< "\n---------------------------------------------------------------------\n";
    cout << "|     COMPUTATIONAL DISCRETE ANALYSIS FRAMEWORK (CDAF)              |\n";
    cout << "---------------------------------------------------------------------\n";
    cout << "\n[MAIN MENU]\n";
    cout << "1. Module 1: Set Theory & Predicate Logic\n";
    cout << "2. Module 2: Graph Algorithms\n";
    cout << "3. Module 3: Advanced Integration\n";
    cout << "4. Select Data Type (Current: Integer)\n";
    cout << "0. Exit\n";
    cout << "\nEnter your choice: ";
}

void displayModule1Menu() {
    cout << "\n[MODULE 1 MENU]\n";
    cout << "1. Test DiscreteSet Operations\n";
    cout << "2. Test Predicate Engine\n";
    cout << "3. Test Graph Constraint Builder\n";
    cout << "0. Back to Main Menu\n";
    cout << "\nEnter your choice: ";
}

void displayModule2Menu() {
    cout << "\n[MODULE 2 MENU]\n";
    cout << "1. Create Network Graph\n";
    cout << "2. Run Kruskal's MST\n";
    cout << "3. Run Dijkstra's Shortest Path\n";
    cout << "0. Back to Main Menu\n";
    cout << "\nEnter your choice: ";
}

void displayModule3Menu() {
    cout << "\n[MODULE 3 MENU]\n";
    cout << "1. Set Up Universe for Constraint Analysis\n";
    cout << "2. Interactive Constraint Analysis\n";
    cout << "3. Integrated Graph Analysis\n";
    cout << "4. Comprehensive Test Suite\n";
    cout << "0. Back to Main Menu\n";
    cout << "\nEnter your choice: ";
}

void displayTypeMenu() {
    cout << "\n[SELECT DATA TYPE]\n";
    cout << "1. Integer (1, 2, 3, ...)\n";
    cout << "2. Character (A, B, C, ...)\n";
    cout << "0. Back to Main Menu\n";
    cout << "\nEnter your choice: ";
}

// ==================== TYPE-SPECIFIC MENU HANDLERS ====================

template<typename T>
void handleModule1() {
    int subChoice;
    while(true) {
        displayModule1Menu();
        cin >> subChoice;
        
        switch(subChoice) {
            case 1: testDiscreteSet<T>(); break;
            case 2: testPredicateEngine<T>(); break;
            case 3: testGraphConstraintBuilder<T>(); break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
        if(subChoice == 0) break;
    }
}

template<typename T>
void handleModule2(NetworkGraph<T>*& graph, AdvancedIntegrationModule<T>& module3) {
    int subChoice;
    while(true) {
        displayModule2Menu();
        cin >> subChoice;
        
        switch(subChoice) {
            case 1: 
                if(graph) delete graph;
                graph = createGraph<T>(); 
                module3.set_current_graph(graph);
                break;
            case 2: runKruskalMST<T>(graph); break;
            case 3: runDijkstraPath<T>(graph); break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
        if(subChoice == 0) break;
    }
}

template<typename T>
void handleModule3(NetworkGraph<T>*& graph, AdvancedIntegrationModule<T>& module3) {
    int subChoice;
    while(true) {
        displayModule3Menu();
        cin >> subChoice;
        
        switch(subChoice) {
            case 1: module3.interactive_universe_setup(); break;
            case 2: module3.interactive_constraint_analysis(); break;
            case 3: module3.integrated_graph_analysis(); break;
            case 4: module3.comprehensive_test_suite(); break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
        if(subChoice == 0) break;
    }
}

// ==================== MAIN FUNCTION ====================

int main() {
    int dataTypeChoice = 1; // Default to integer
    int choice, subChoice;
    
    // Pointers for different data types
    NetworkGraph<int>* intGraph = nullptr;
    NetworkGraph<char>* charGraph = nullptr;
    AdvancedIntegrationModule<int> intModule3;
    AdvancedIntegrationModule<char> charModule3;
    
    while(true) {
        displayMainMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: // Module 1
                if(dataTypeChoice == 1) {
                    handleModule1<int>();
                } else {
                    handleModule1<char>();
                }
                break;
                
            case 2: // Module 2
                if(dataTypeChoice == 1) {
                    handleModule2<int>(intGraph, intModule3);
                } else {
                    handleModule2<char>(charGraph, charModule3);
                }
                break;
                
            case 3: // Module 3
                if(dataTypeChoice == 1) {
                    handleModule3<int>(intGraph, intModule3);
                } else {
                    handleModule3<char>(charGraph, charModule3);
                }
                break;
                
            case 4: // Select Data Type
                displayTypeMenu();
                cin >> dataTypeChoice;
                if(dataTypeChoice == 1 || dataTypeChoice == 2) {
                    cout << "\nData type set to " << (dataTypeChoice == 1 ? "INTEGER" : "CHARACTER") << endl;
                    // Clean up old data if switching types
                    if(dataTypeChoice == 1) {
                        if(charGraph) {
                            delete charGraph;
                            charGraph = nullptr;
                        }
                    } else {
                        if(intGraph) {
                            delete intGraph;
                            intGraph = nullptr;
                        }
                    }
                } else if(dataTypeChoice != 0) {
                    cout << "Invalid choice!\n";
                }
                break;
                
            case 0:
                cout << "\nThank you for using CDAF! Goodbye.\n";
                // Clean up
                if(intGraph) delete intGraph;
                if(charGraph) delete charGraph;
                return 0;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}