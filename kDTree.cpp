#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
kDTree::kDTree(int k){
    this->k = k;
    this->root = nullptr;
}
kDTree::~kDTree(){
    if(this->root != nullptr){
        delete this->root;
    }
}
kDTree::kDTree(const kDTree &other){
    this->k = other.k;
    this->root = other.root;
}
const kDTree &kDTree::operator=(const kDTree &other){
    if(this == &other){
        return *this;
    }
    this->k = other.k;
    this->root = other.root;
    return *this;
}
kNN::kNN(int k){
    this->k = k;
    this->tree = nullptr;
}
kNN::~kNN(){
    if(this->tree != nullptr){
        delete this->tree;
    }
}
void rec_inorderTraversal(kDTreeNode * node, int k) {
    if (node == nullptr) {
        return;
    }

    // Traverse the left subtree
    rec_inorderTraversal(node->left, k);

    // Visit the current node
    std::cout << "(";
    int *point=new int[k];
    copy(node->data.begin(), node->data.end(), point);
    for (int i = 0; i < k; ++i) {
        cout << point[i];
        if (i != k - 1) {
            cout << ", ";
        }
    }
    std::cout << ") ";

    // Traverse the right subtree
    rec_inorderTraversal(node->right, k);
}
void rec_preorderTraversal(kDTreeNode * node, int k) {
    if (node == nullptr) {
        return;
    }

    // Visit the current node
    std::cout << "(";
    int* point=new int[k];
    copy(node->data.begin(), node->data.end(), point);
    for (int i = 0; i < k; ++i) {
        cout << point[i];
        if (i != k - 1) {
            cout << ", ";
        }
    }
    std::cout << ") ";
    // Traverse the left subtree
    rec_preorderTraversal(node->left, k);

    // Traverse the right subtree
    rec_preorderTraversal(node->right, k);
}
void rec_postorderTraversal(kDTreeNode * node, int k) {
    if (node == nullptr) {
        return;
    }

    // Traverse the left subtree
    rec_postorderTraversal(node->left, k);

    // Traverse the right subtree
    rec_postorderTraversal(node->right, k);

    // Visit the current node
    std::cout << "(";
    int *point= new int[k];
    copy(node->data.begin(), node->data.end(), point);
    for (int i = 0; i < k; ++i) {
        cout << point[i];
        if (i != k - 1) {
            cout << ", ";
        }
    }
    std::cout << ") ";
}
int rec_height(kDTreeNode * node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = rec_height(node->left);
    int rightHeight = rec_height(node->right);
    return 1 + max(leftHeight, rightHeight);
}
int rec_nodeCount(kDTreeNode * node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + rec_nodeCount(node->left) + rec_nodeCount(node->right);
}
int rec_leafCount(kDTreeNode * node) {
    if (node == nullptr) {
        return 0;
    }
    if (node->left == nullptr && node->right == nullptr) {
        return 1;
    }
    return rec_leafCount(node->left) + rec_leafCount(node->right);
}
void rec_insert(kDTreeNode *&node, const vector<int> &point, int k, int depth){
    if(node == nullptr){
        node = new kDTreeNode(point);
        return;
    }
    int axis = depth % k;
    if(point[axis] < node->data[axis]){
        rec_insert(node->left, point, k, depth + 1);
    }else{
        rec_insert(node->right, point, k, depth + 1);
    }
}
bool compareVectors(const vector<int> &vec1, const vector<int> &vec2) {
    int size = vec1.size();
    if (size != vec2.size()) {
        return false; // Nếu số lượng phần tử không bằng nhau, hai vector không giống nhau
    }

    for (size_t i = 0; i < size; ++i) {
        if (vec1[i] != vec2[i]) {
            return false; // Nếu có ít nhất một cặp phần tử không giống nhau, hai vector không giống nhau
        }
    }

    return true; // Nếu không có cặp phần tử nào khác nhau, hai vector giống nhau
}
bool rec_search(kDTreeNode *node, const vector<int> &point, int k, int depth){
    if(node == nullptr){
        return false;
    }
    if(compareVectors(node->data, point)){
        return true;
    }
    int axis = depth % k;
    if(point[axis] < node->data[axis]){
        return rec_search(node->left, point, k, depth + 1);
    }else{
        return rec_search(node->right, point, k, depth + 1);
    }
}
/*Quá trình xóa của chúng ta sẽ hoạt động như sau. Quá trình này là đệ quy, giả định node
cần xóa là u:
(a) Nếu u là một lá: Chỉ cần xóa nó và kết thúc.
(b) Nếu u có cây con bên phải: Đặt α là chiều mà u phân chia. Tìm một node thay thế r
trong u.right (cây con bên phải của u) sao cho giá trị theo chiều α của r là nhỏ nhất.
Chúng ta copy điểm r vào u (ghi đè lên điểm của u). Sau đó, chúng ta gọi đệ quy xóa
trên node r cũ.
(c) Nếu u không có cây con bên phải thì nó sẽ có cây con bên trái: Đặt α là chiều mà u phân
chia. Tìm một node thay thế r trong u.left (cây con bên trái) sao cho giá trị theo chiều α
của r là nhỏ nhất. Chúng ta copy điểm r vào u (ghi đè lên điểm của u) và sau đó, chúng
ta di chuyển cây con bên trái của u để trở thành cây con bên phải của u. Sau đó, chúng ta gọi đệ quy xóa trên node r cũ*/
kDTreeNode *minNode(kDTreeNode *x, kDTreeNode *y, kDTreeNode *z, int axis){
    kDTreeNode *res = x;
    if(y != nullptr && y->data[axis] < res->data[axis]){
        res = y;
    }
    if(z != nullptr && z->data[axis] < res->data[axis]){
        res = z;
    }
    return res;
}
kDTreeNode *findMin(kDTreeNode *node, int k, int axis, int depth){
    if(node == nullptr){
        return nullptr;
    }
    int currentAxis = depth % k;
    if(currentAxis == axis){
        if(node->left == nullptr){
            return node;
        }
        return findMin(node->left, k, axis, depth + 1);
    }
    return minNode(node, findMin(node->left, k, axis, depth + 1), findMin(node->right, k, axis, depth + 1), axis);
}

void rec_remove(kDTreeNode *&node, const vector<int> &point, int k, int depth){
    if(node == nullptr){
        return;
    }
    int axis = depth % k;
    if(compareVectors(node->data, point)){
        if(node->right != nullptr){
            kDTreeNode *min = findMin(node->right, k, axis, depth + 1);
            node->data = min->data;
            rec_remove(node->right, min->data, k, depth + 1);
        }else if(node->left != nullptr){
            kDTreeNode *min = findMin(node->left, k, axis, depth + 1);
            node->data = min->data;
            rec_remove(node->left, min->data, k, depth + 1);
        }else{
            delete node;
            node = nullptr;
        }
    }else if(point[axis] < node->data[axis]){
        rec_remove(node->left, point, k, depth + 1);
    }else{
        rec_remove(node->right, point, k, depth + 1);
    }
}


//buildTree balanced. Để có thể xây một cây k-D cân bằng, ta tuân theo giải thuật sau:
/* Để có thể xây một cây k-D cân bằng, ta tuân theo giải thuật sau:
1. Khi di chuyển xuống cây, xác định mặt phẳng chia hiện tại.
2. Sắp xếp lại danh sách các điểm theo chiều của mặt phẳng chia hiện tại.
3. Xác định điểm tiếp theo được chèn vào là trung vị của danh sách sau khi được sắp xếp
4. Cây con bên trái của điểm mới được thêm vào sẽ là cây được xây bằng danh sách các
điểm bé hơn điểm trung vị. Cây con bên phải của điểm mới được thêm vào sẽ là cây được
xây bằng danh sách các điểm lớn hơn hoặc bằng điểm trung vị.
Chú ý rằng, để tìm ra điểm trung vị, ta có thể sử dụng nhiều phương pháp khác nhau.
BTL này yêu cầu sinh viên phải tìm điểm trung vị bằng cách sử dụng Mergesort để sắp xếp
lại danh sách, sau đó chọn điểm chính giữa là điểm trung vị. Nếu có 2 điểm chính giữa, ta sẽ
chọn điểm đầu tiên trong 2 điểm đó.
*/
//Mergesort
void merge(vector<vector<int>> &pointList, int l, int m, int r, int axis){
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<vector<int>> L(n1), R(n2);
    for(int i = 0; i < n1; i++){
        L[i] = pointList[l + i];
    }
    for(int i = 0; i < n2; i++){
        R[i] = pointList[m + 1 + i];
    }
    int i = 0, j = 0, k = l;
    while(i < n1 && j < n2){
        if(L[i][axis] <= R[j][axis]){
            pointList[k] = L[i];
            i++;
        }else{
            pointList[k] = R[j];
            j++;
        }
        k++;
    }
    while(i < n1){
        pointList[k] = L[i];
        i++;
        k++;
    }
    while(j < n2){
        pointList[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(vector<vector<int>> &pointList, int l, int r, int axis){
    if(l >= r){
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort(pointList, l, m, axis);
    mergeSort(pointList, m + 1, r, axis);
    merge(pointList, l, m, r, axis);
}
void rec_buildTree(kDTreeNode *&node, vector<vector<int>> &pointList, int k, int depth){
    if(pointList.empty()){
        return;
    }
    int axis = depth % k;
    mergeSort(pointList, 0, pointList.size() - 1, axis);
    int mid = (pointList.size()-1)/ 2;
    node = new kDTreeNode(pointList[mid]);
    vector<vector<int>> leftPointList(pointList.begin(), pointList.begin() + mid);
    vector<vector<int>> rightPointList(pointList.begin() + mid + 1, pointList.end());
    rec_buildTree(node->left, leftPointList, k, depth + 1);
    rec_buildTree(node->right, rightPointList, k, depth + 1);
}
void merge_labled(vector<vector<int>> &pointList, vector<int> &labelList, int l, int m, int r, int axis){
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<vector<int>> L(n1);
    vector<int> L_label(n1);
    vector<vector<int>> R(n2);
    vector<int> R_label(n2);
    for(int i = 0; i < n1; i++){
        L[i] = pointList[l + i];
        L_label[i] = labelList[l + i];
    }
    for(int i = 0; i < n2; i++){
        R[i] = pointList[m + 1 + i];
        R_label[i] = labelList[m + 1 + i];
    }
    int i = 0, j = 0, k = l;
    while(i < n1 && j < n2){
        if(L[i][axis] <= R[j][axis]){
            pointList[k] = L[i];
            labelList[k] = L_label[i];
            i++;
        }else{
            pointList[k] = R[j];
            labelList[k] = R_label[j];
            j++;
        }
        k++;
    }
    while(i < n1){
        pointList[k] = L[i];
        labelList[k] = L_label[i];
        i++;
        k++;
    }
    while(j < n2){
        pointList[k] = R[j];
        labelList[k] = R_label[j];
        j++;
        k++;
    }
}
void mergeSort_labled(vector<vector<int>> &pointList, vector<int> &labelList, int l, int r, int axis){
    if(l >= r){
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort_labled(pointList, labelList, l, m, axis);
    mergeSort_labled(pointList, labelList, m + 1, r, axis);
    merge_labled(pointList, labelList, l, m, r, axis);
}
void rec_buildTree_labled(kDTreeNode *&node, vector<vector<int>> &pointList, vector<int> &labelList, int k, int depth){
    if(pointList.empty()){
        return;
    }
    int axis = depth % k;
    mergeSort_labled(pointList, labelList, 0, pointList.size() - 1, axis);
    int mid = (pointList.size()-1) / 2;
    node = new kDTreeNode(pointList[mid]);
    node->label = labelList[mid];
    vector<vector<int>> leftPointList(pointList.begin(), pointList.begin() + mid);
    vector<int> leftLabelList(labelList.begin(), labelList.begin() + mid);
    vector<vector<int>> rightPointList(pointList.begin() + mid + 1, pointList.end());
    vector<int> rightLabelList(labelList.begin() + mid + 1, labelList.end());
    rec_buildTree_labled(node->left, leftPointList, leftLabelList, k, depth + 1);
    rec_buildTree_labled(node->right, rightPointList, rightLabelList, k, depth + 1);
}
int distance(const vector<int> &point1, const vector<int> &point2){
    int size = point1.size();
    int sum = 0;
    for(int i = 0; i < size; i++){
        sum += (point1[i] - point2[i]) * (point1[i] - point2[i]);
    }
    return sum;
}
void kDTree::inorderTraversal() const{
    rec_inorderTraversal(this->root, this->k);
}
void kDTree::preorderTraversal() const{
    rec_preorderTraversal(this->root, this->k);
}
void kDTree::postorderTraversal() const{
    rec_postorderTraversal(this->root, this->k);
}
int kDTree::height() const{
    return rec_height(this->root);
}
int kDTree::nodeCount() const{
    return rec_nodeCount(this->root);
}
int kDTree::leafCount() const{
    return rec_leafCount(this->root);
}
void kDTree::insert(const vector<int> &point){
    if(this->k != point.size()){
        return;
    }
    rec_insert(this->root, point, this->k, 0);
}
bool kDTree::search(const vector<int> &point){
    if(this->k != point.size()){
        return false;
    }
    return rec_search(this->root, point, this->k, 0);
}

void kDTree::buildTree(const vector<vector<int>> &pointList){
    vector<vector<int>> pointListCopy;
    int size = pointList.size();
    this->k = pointList[0].size();
    for(int i = 0; i < size; i++){
        pointListCopy.push_back(pointList[i]);
    }
    rec_buildTree(this->root, pointListCopy, this->k, 0);
}
void kDTree::buildTree_labled(const vector<vector<int>> &pointList, const vector<int> &labelList){
    vector<vector<int>> pointListCopy;
    vector<int> labelListCopy;
    int size = pointList.size();
    this->k = pointList[0].size();
    for(int i = 0; i < size; i++){
        pointListCopy.push_back(pointList[i]);
        labelListCopy.push_back(labelList[i]);
    }
    rec_buildTree_labled(this->root, pointListCopy, labelListCopy, this->k, 0);
}
void kDTree::remove(const vector<int> &point){
    rec_remove(this->root, point, this->k, 0);
}
/*Việc tìm kiếm điểm láng giềng gần nhất trong một cây k-d diễn ra như sau:
1. Bắt đầu tại node gốc của cây
2. Di chuyển xuống cây theo đệ quy, tương tự như khi chèn một điểm, bằng cách so sánh
tọa độ của điểm hiện tại với mặt phẳng chia tại mỗi node.
3. Khi đến một node lá (hoặc là không di chuyển được nữa), lưu trữ nó như là láng giềng
gần nhất tiềm năng.
4. Đi ngược trở lại lên trên cây, kiểm tra từng node:
• Nếu node hiện tại gần hơn láng giềng gần nhất đã lưu trữ, cập nhật láng giềng gần
nhất.
• Kiểm tra xem liệu có thể có các điểm gần hơn ở phía bên kia của mặt phẳng tách:
– Hãy tưởng tượng một hình cầu xung quanh điểm tìm kiếm có bán kính bằng
khoảng cách đến láng giềng gần nhất hiện tại. (1)
– Nếu hình cầu này cắt mặt phẳng tách tại node hiện tại, hãy khám phá phía bên
kia của cây từ node đó. (2)
– Nếu không, tiếp tục đi ngược lên trên cây, loại bỏ nhánh ở phía bên kia.
5. Lặp lại quá trình này cho đến khi đến node gốc, hoàn thành tìm kiếm*/
// void rec_nearestNeighbour(kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int k, int depth){
//     if(node == nullptr){
//         return;
//     }
//     int axis = depth % k;
//     kDTreeNode *next = nullptr;
//     kDTreeNode *opposite = nullptr;
//     if(target[axis] < node->data[axis]){
//         next = node->left;
//         opposite = node->right;
//     }else{
//         next = node->right;
//         opposite = node->left;
//     }
//     rec_nearestNeighbour(next, target, best, k, depth + 1);
//     if(distance(node->data, target) < distance(best->data, target)){
//         best = node;
//     }
//     if(distance(node->data, target) > abs(target[axis] - node->data[axis])){
//         rec_nearestNeighbour(opposite, target, best, k, depth + 1);
//     }
// }
void rec_nearestNeighbour(kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int k, int depth){
    if(node == nullptr){
        return;
    }
    int axis = depth % k;
    kDTreeNode *next = nullptr;
    kDTreeNode *opposite = nullptr;
    if(target[axis] < node->data[axis]){
        next = node->left;
        opposite = node->right;
    }else{
        next = node->right;
        opposite = node->left;
    }
    rec_nearestNeighbour(next, target, best, k, depth + 1);
    if(best == nullptr){
        best = node;
    }else{
        if(distance(node->data, target) < distance(best->data, target)){
            best = node;
        }
    }
    if(distance(node->data, target) > abs(target[axis] - node->data[axis])){
        rec_nearestNeighbour(opposite, target, best, k, depth + 1);
    }
}
void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best){
    rec_nearestNeighbour(this->root, target, best, this->k, 0);
}
/*
Tìm k lá hàng xóm gần nhất:
    Bước 1: Khởi tạo một hàng đợi ưu tiên (priority queue) gọi là "Best" để lưu trữ k lá hàng xóm tốt nhất.
    Bước 2: Bắt đầu từ gốc của cây kD, duyệt cây kD để tìm vị trí mà điểm truy vấn thuộc về.
    Bước 3: Tìm lá hàng xóm gần nhất bằng cách đi xuống cây kD cho đến khi đạt tới một lá. Lưu lá này làm phần tử đầu tiên trong hàng đợi "Best".
    Bước 4: Duyệt lên trên từ lá hàng xóm đã tìm được và cho mỗi nút gặp phải:
        Đặt nút đó vào vị trí phù hợp trong hàng đợi "Best" dựa trên khoảng cách đến điểm truy vấn.
        Kiểm tra xem có thể có lá hàng xóm tốt hơn ở phía bên kia cây kD không:
            Xác định một hình cầu xung quanh điểm truy vấn với bán kính bằng khoảng cách đến phần tử cuối cùng trong hàng đợi "Best".
            Kiểm tra xem hình cầu có cắt qua mặt phẳng chia tại điểm phân nhánh không.
            Nếu có thể có, đi xuống cây kD phía bên kia. Nếu không, đi lên một cấp độ khác.
    Bước 5: Trả về k lá hàng xóm tốt nhất từ hàng đợi "Best".
*/
void sortBestList(vector<kDTreeNode *> &bestList, const vector<int> &target){
    int size = bestList.size();
    int *distanceList=new int[size];
    for(int i = 0; i < size; i++){
        distanceList[i] = distance(bestList[i]->data, target);
    }
    for(int i = 0; i < size; i++){
        for(int j = i + 1; j < size; j++){
            if(distanceList[i] > distanceList[j]){
                swap(bestList[i], bestList[j]);
                swap(distanceList[i], distanceList[j]);
            }
        }
    }
    delete[] distanceList;
}
/*Instead of keeping a single closest point we could maintain a priority queue (max heap) to keep k (say 2, 3 or any number) closest points. 
The first k points would be en-queued anyway. Onwards, a new point, if better, would replace the worst of the closest point found so far. 
That way we can maintain the k nearest points easily*/
// build a max heap with vector<kDTreeNode *> bestList
void reHeapUp(vector<kDTreeNode *> &bestList, int index){
    // if(index == 0){
    //     return;
    // }
    // int parent = (index - 1) / 2;
    // if(bestList[parent]->distance < bestList[index]->distance){
    //     swap(bestList[parent], bestList[index]);
    //     reHeapUp(bestList, parent);
    // }
    int parent = (index - 1) / 2;
    kDTreeNode* bottom = bestList[index];
    while (index > 0 && bestList[parent]->distance < bottom->distance) {
        bestList[index] = bestList[parent];
        index = parent;
        parent = (parent - 1) / 2;
    }
    bestList[index] = bottom;
}
void reHeapDown(vector<kDTreeNode *> &bestList, int index){
    // int left = 2 * index + 1;
    // int right = 2 * index + 2;
    // int largest = index;
    // if(left < bestList.size() && bestList[left]->distance > bestList[largest]->distance){
    //     largest = left;
    // }
    // if(right < bestList.size() && bestList[right]->distance > bestList[largest]->distance){
    //     largest = right;
    // }
    // if(largest != index){
    //     swap(bestList[largest], bestList[index]);
    //     reHeapDown(bestList, largest);
    // }
    kDTreeNode* top = bestList[index];
    int largerChild;
    int numberOfElements = bestList.size();
    while (index < numberOfElements / 2) {
        int leftChild = 2 * index + 1;
        int rightChild = leftChild + 1;
        if (rightChild < numberOfElements && bestList[leftChild]->distance < bestList[rightChild]->distance) {
            largerChild = rightChild;
        } else {
            largerChild = leftChild;
        }
        if (top->distance >= bestList[largerChild]->distance) {
            break;
        }
        bestList[index] = bestList[largerChild];
        index = largerChild;
    }
    bestList[index] = top;

}
void rec_kNearestNeighbour(kDTreeNode *node, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int depth, int d){
    if(node == nullptr){
        return;
    }
    int axis = depth % d;
    kDTreeNode *next = nullptr;
    kDTreeNode *opposite = nullptr;
    if(target[axis] < node->data[axis]){
        next = node->left;
        opposite = node->right;
    }else{
        next = node->right;
        opposite = node->left;
    }
    rec_kNearestNeighbour(next, target, k, bestList, depth + 1, d);
    if(bestList.size() < k){
        node->distance = distance(node->data, target);
        bestList.push_back(node);
        reHeapUp(bestList, bestList.size() - 1);
    }else{
        node->distance = distance(node->data, target);
        if(node->distance < bestList[0]->distance){

            bestList[0] = bestList[k - 1];
            reHeapDown(bestList, 0);
            bestList[k-1]=node;
           // reHeapDown(bestList, 0);
            reHeapUp(bestList, k-1);
        }
    }
    // for(int i = 0; i < bestList.size(); i++){
    //     cout << bestList[i]->distance << " ";
    // }
    if(distance(node->data, target) > abs(target[axis] - node->data[axis])){
        rec_kNearestNeighbour(opposite, target, k, bestList, depth + 1, d);
    }
}
void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList){
    int d = target.size();
    if(this->k!=d){
        return;
    }
    rec_kNearestNeighbour(this->root, target, k, bestList, 0, d);
    //     for(int i = 0; i < k; i++){
    //     cout << bestList[i]->label<< " ";
    // }
    // cout<<endl;
    sortBestList(bestList, target);

    // for(int i = 0; i < k; i++){
    //     cout << bestList[i]->label<< " ";
    // }
    // cout<<endl;
}

// void rec_kNearestNeighbour(kDTreeNode *node, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int depth, int d){
//     if(node == nullptr){
//         return;
//     }
//     int axis = depth % d;
//     kDTreeNode *next = nullptr;
//     kDTreeNode *opposite = nullptr;
//     if(target[axis] < node->data[axis]){
//         next = node->left;
//         opposite = node->right;
//     }else{
//         next = node->right;
//         opposite = node->left;
//     }
//     rec_kNearestNeighbour(next, target, k, bestList, depth + 1, d);
//     if(bestList.size() < k){
//         bestList.push_back(node);
//     }else{
//         if(distance(node->data, target) < distance(bestList[k - 1]->data, target)){
//             bestList[k - 1] = node;
//         }
//     }
//     if(distance(node->data, target) > abs(target[axis] - node->data[axis])){
//         rec_kNearestNeighbour(opposite, target, k, bestList, depth + 1, d);
//     }
// }
// void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList){
//     int d = target.size();
//     if(this->k!=d){
//         return;
//     }
//     rec_kNearestNeighbour(this->root, target, k, bestList, 0, d);
//     //sort bestList
//     //sortBestList(bestList, target);
//     for(int i = 0; i < k; i++){
//         cout << bestList[i]->label<< " ";
//     }
//     cout<<endl;

// }
void kNN::fit(Dataset &X_train, Dataset &y_train){
    int nRows, nCols;
    X_train.getShape(nRows, nCols);
    if(tree != nullptr){
        delete tree;
    }
    vector<vector<int>> pointList;
    vector<int> labelList;
    for(const auto& inner_list:X_train.data){
        vector <int> inner_vec(inner_list.begin(), inner_list.end());
        pointList.push_back(inner_vec);
    }
    for(const auto& inner_list:y_train.data){
        vector <int> inner_vec(inner_list.begin(), inner_list.end());
        labelList.push_back(inner_vec[0]);
    }
    // for(int i = 0; i < nRows; i++){
    //     cout<<labelList[i]<<" ";
    // }
    this->tree = new kDTree(nCols);
    this->tree->buildTree_labled(pointList, labelList);
    // this->tree->inorderTraversal_in_file();
    // cout<<endl;
    
    // kDTree *tmp=new kDTree(nCols);
    // tmp->buildTree(pointList);
    // tmp->inorderTraversal_in_file();
    // delete tmp;
}
Dataset kNN::predict(Dataset &X_test){
    Dataset result;
    result.columnName = {"label"};
    int nRows, nCols;
    X_test.getShape(nRows, nCols);
    vector<vector<int>> pointList;
    for(const auto& inner_list:X_test.data){
        vector <int> inner_vec(inner_list.begin(), inner_list.end());
        pointList.push_back(inner_vec);
    }
    int predict_label[10];
    for(int i = 0; i < nRows; i++){
        vector<kDTreeNode *> best;
        this->tree->kNearestNeighbour(pointList[i], this->k, best);
        // cout<< "best size: "<<best.size()<<endl;
        for(int j = 0; j < 10; j++){
            predict_label[j] = 0;
        }
        for(int j = 0; j < this->k; j++){
            predict_label[best[j]->label]++;
        }
        // int max = best[0]->label;
        int max = 0;
        for(int j = 0; j < 10; j++){
            if(predict_label[j] > predict_label[max]){
                max = j;
            }
        }
        result.data.push_back({max});
    }
    return result;
}
double kNN::score(const Dataset &y_test, const Dataset &y_pred){
    int nRows, nCols;
    y_test.getShape(nRows, nCols);
    int count = 0;
    vector<vector<int>> y_test_data;
    vector<vector<int>> y_pred_data;
    for(const auto& inner_list:y_test.data){
        vector <int> inner_vec(inner_list.begin(), inner_list.end());
        y_test_data.push_back(inner_vec);
    }
    for(const auto& inner_list:y_pred.data){
        vector <int> inner_vec(inner_list.begin(), inner_list.end());
        y_pred_data.push_back(inner_vec);
    }
    for(int i = 0; i < nRows; i++){
        if(y_test_data[i][0] == y_pred_data[i][0]){
            count++;
        }
    }
    return (double)count / nRows;
} 










