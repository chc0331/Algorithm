#include <stdio.h>
#include <stdlib.h>

#define size 100000

//map

struct point {
	int x, y;
};

point p[size];
int n;
point sort[size];

void merge(int left, int mid, int right) {
	int i, j, k, l;
	i = left; j = mid + 1; k = left;

	while (i <= mid && j <= right) {
		if (p[i].x < p[j].x) {
			sort[k].x = p[i].x; sort[k].y = p[i].y;
			k++; i++;
		}
		else {
			sort[k].x = p[j].x; sort[k].y = p[j].y;
			k++; j++;
		}
	}

	if (i > mid) {
		for (l = j; l <= right; l++) {
			sort[k].x = p[l].x; sort[k].y = p[l].y;
			k++;
		}
	}
	else {
		for (l = i; l <= mid; l++) {
			sort[k].x = p[l].x; sort[k].y = p[l].y;
			k++;
		}
	}

	for (l = left; l <= right; l++) {
		p[l].x = sort[l].x; p[l].y = sort[l].y;
	}
}

void merge_sort(int left, int right) {
	if (left < right) {
		int mid = (left + right) / 2;
		merge_sort(left, mid);
		merge_sort(mid + 1, right);
		merge(left, mid, right);
	}
}

int dist(point* p1, point* p2) {
	return (p1->x - p2->x) * (p1->x - p2->x)
		+ (p1->y - p2->y) * (p1->y - p2->y);
}

int min(int a, int b) {
	if (a < b)return a;
	else return b;
}

int ans;

struct p_list {
	p_list* next;
	int idx;
};

p_list p_arr[size];
int p_idx;

struct node {
	node* left;
	node* right;
	int key;
	p_list* list;
};

node* root;

node node_list[size];
int node_idx;

int p_delete[size];
int p_len;

node* new_node(int key, int idx) {
	node* mnode = &node_list[node_idx++];
	mnode->left = nullptr;
	mnode->right = nullptr;
	mnode->key = key;

	p_list* plist = &p_arr[p_idx++];
	plist->idx = idx; plist->next = nullptr;

	mnode->list = plist;

	return mnode;
}

node* insert(node* cur, int key, int idx) {

	if (cur == nullptr) {
		if (cur == root)
			return root = new_node(key, idx);

		return new_node(key, idx);
	}
	else {
		if (key < cur->key) {
			cur->left = insert(cur->left, key, idx);
		}
		else if (key > cur->key) {
			cur->right = insert(cur->right, key, idx);
		}
		else {
			p_list* new_p = &p_arr[p_idx++];
			new_p->idx = idx;
			new_p->next = cur->list;
			cur->list = new_p;
		}

		return cur;
	}
}

node* minNode(node* cur) {

	while (1) {
		if (cur->left == nullptr)break;

		cur = cur->left;
	}

	return cur;
}

void printall(node* cur) {

	if (cur == nullptr)return;

	printall(cur->left);
	printf("기준 %d\n", cur->key);
	p_list* list = cur->list;
	while (list != nullptr) {
		printf("%d ", list->idx);

		list = list->next;
	}
	printall(cur->right);
}

node* remove(node* cur, int key, int idx) {

	if (key < cur->key) {
		cur->left = remove(cur->left, key, idx);
	}
	else if (key > cur->key) {
		cur->right = remove(cur->right, key, idx);
	}
	else {
		p_list* list = cur->list;
		p_list* before = nullptr;

		while (list != nullptr) {
			if (list->idx == idx) {
				if (list == cur->list) {
					cur->list = cur->list->next;
					//노드삭제
					if (cur->list == nullptr) {

						if (cur->left == nullptr)return cur->right;
						else if (cur->right == nullptr)return cur->left;
						else {

							node* minnode = minNode(cur->right);

							cur->key = minnode->key;
							cur->list = minnode->list;
							minnode->list->next = nullptr;

							remove(cur->right, minnode->key, minnode->list->idx);
						}
					}

					return cur;
				}
				else {
					before->next = list->next;
					return cur;
				}
			}

			before = list;
			list = list->next;
		}
	}

	return cur;
}

//pivot과의 x좌표 차이가 ans보다 크다면 빼주기 위해서 배열에 삽입
void traverse(node* cur, int pivot) {

	if (cur == nullptr)return;

	p_list* list = cur->list;
	while (list != nullptr) {
		if (p[list->idx].x > pivot) {
			if (p[list->idx].x - pivot >= ans)
				p_delete[p_len++] = list->idx;
		}
		else {
			if (pivot - p[list->idx].x >= ans)
				p_delete[p_len++] = list->idx;
		}

		list = list->next;
	}

	traverse(cur->left, pivot);
	traverse(cur->right, pivot);
}

//ans update 메서드
void update(node* cur, int idx) {

	if (cur == nullptr)return;

	if (p[idx].y > cur->key) {
		int distance = p[idx].y - cur->key;

		if (distance <= ans) {
			p_list* list = cur->list;
			while (list != nullptr) {
				ans = min(ans, dist(&p[list->idx], &p[idx]));
				list = list->next;
			}
			update(cur->left, idx);update(cur->right, idx);
		}
		else {
			update(cur->right, idx);
		}
	}
	else {
		int distance = cur->key - p[idx].y;

		if (distance <= ans) {
			p_list* list = cur->list;
			while (list != nullptr) {
				ans = min(ans, dist(&p[list->idx], &p[idx]));
				list = list->next;
			}
			update(cur->left, idx);update(cur->right, idx);
		}
		else {
			update(cur->left, idx);
		}

	}
}

int main(void) {

	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		int x = rand() % 107;
		int y = rand() % 107;

		//scanf("%d %d", &p[i].x, &p[i].y);

		p[i].x = x;p[i].y = y;

		printf("%d %d\n", x, y);
	}

	//x좌표 기준으로 포인터들 정렬
	merge_sort(0, n - 1);

	//최초 짧은 거리 기준
	ans = dist(&p[0], &p[1]);
	insert(root, p[0].y, 0);
	insert(root, p[1].y, 1);

	printf("start\n");
	printf("0번 %d %d\n", p[0].x, p[0].y);
	printf("1번 %d %d\n", p[1].x, p[1].y);
	for (int i = 2; i < n; i++) {
		//후보자들을 돌면서 현재 포인트와 x 좌표 차이가
		//ans보다 크면 후보자에서 제외
		printf("%d번 %d %d\n", i, p[i].y, p[i].x);
		//x좌표 차이가 ans보다 큰 후보자들 추출
		p_len = 0;
		traverse(root, p[i].x);
		//추출한 후보자들 제거
		for (int j = 0;j < p_len;j++) {
			int pidx = p_delete[j];
			remove(root, p[pidx].y, pidx);
		}

		printf("remove\n");

		//후보자들중에서 현재 포인트와 y좌표 차이가
		//ans보다 작은거를 돌면서 ans 갱신
		update(root, i);

		//현재 좌표 추가
		insert(root, p[i].y, i);

		printf("%d %d\n", i, ans);
	}

	printf("%d\n", ans);
}