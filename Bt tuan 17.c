#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int du_lieu;        
    struct Node* trai; 
    struct Node* phai;
    int chieu_cao;
} Node;

// lấy chiều cao của nút
int layChieuCao(Node* nut) {
    if (nut == NULL)
        return 0;
    return nut->chieu_cao;
}

// tính hệ số cân bằng của nút
int tinhHeSoCanBang(Node* nut) {
    if (nut == NULL)
        return 0;
    return layChieuCao(nut->trai) - layChieuCao(nut->phai);
}

// tạo nút mới
Node* taoNutMoi(int du_lieu) {
    Node* nutMoi = (Node*)malloc(sizeof(Node));
    if (nutMoi == NULL) {
        printf("Lỗi cấp phát bộ nhớ!\n");
        exit(1);
    }
    nutMoi->du_lieu = du_lieu;
    nutMoi->trai = NULL;
    nutMoi->phai = NULL;
    nutMoi->chieu_cao = 1; // Chiều cao ban đầu nút mới
    return nutMoi;
}

//cập nhật chiều cao của một nút
void capNhatChieuCao(Node* nut) {
    if (nut != NULL) {
        int chieuCaoTrai = layChieuCao(nut->trai);
        int chieuCaoPhai = layChieuCao(nut->phai);
        nut->chieu_cao = (chieuCaoTrai > chieuCaoPhai ? chieuCaoTrai : chieuCaoPhai) + 1;
    }
}

Node* xoayPhai(Node* y) {
    Node* x = y->trai;
    Node* T2 = x->phai;

    x->phai = y;
    y->trai = T2;

    // Cập nhật chiều cao
    capNhatChieuCao(y);
    capNhatChieuCao(x);

    // trả nút gốc mới
    return x;
}


Node* xoayTrai(Node* x) {
    Node* y = x->phai;
    Node* T2 = y->trai;

    y->trai = x;
    x->phai = T2;

    capNhatChieuCao(x);
    capNhatChieuCao(y);

    // Trả nút gốc mới
    return y;
}


Node* canBang(Node* nut) {
    if (nut == NULL) return nut;

    capNhatChieuCao(nut);

    int heSoCanBang = tinhHeSoCanBang(nut);


    if (heSoCanBang > 1) {
        if (tinhHeSoCanBang(nut->trai) < 0) {
            nut->trai = xoayTrai(nut->trai);
        }
        return xoayPhai(nut);
    }


    if (heSoCanBang < -1) {
        if (tinhHeSoCanBang(nut->phai) > 0) {
            nut->phai = xoayPhai(nut->phai);
        }
        return xoayTrai(nut);
    }
    return nut;
}


Node* chen(Node* nut, int du_lieu) {

    if (nut == NULL)
        return taoNutMoi(du_lieu);

    if (du_lieu < nut->du_lieu)
        nut->trai = chen(nut->trai, du_lieu);
    else if (du_lieu > nut->du_lieu)
        nut->phai = chen(nut->phai, du_lieu);
    else // Không dữ liệu trùng lặp
        return nut;

 
    return canBang(nut);
}


// duyệt cây theo thứ tự trước 
void duyetTruoc(Node* nut) {
    if (nut != NULL) {
        printf("%d ", nut->du_lieu);
        duyetTruoc(nut->trai);
        duyetTruoc(nut->phai);
    }
}

//duyệt cây theo thứ tự giữa 
void duyetGiua(Node* nut) {
    if (nut != NULL) {
        duyetGiua(nut->trai);
        printf("%d ", nut->du_lieu);
        duyetGiua(nut->phai);
    }
}

// duyệt cây theo thứ tự sau 
void duyetSau(Node* nut) {
    if (nut != NULL) {
        duyetSau(nut->trai);
        duyetSau(nut->phai);
        printf("%d ", nut->du_lieu);
    }
}

// giải phóng bộ nhớ cấp cho cây
void giaiPhongCay(Node* nut) {
    if (nut != NULL) {
        giaiPhongCay(nut->trai);
        giaiPhongCay(nut->phai);
        free(nut);
    }
}

int main() {
    Node* goc = NULL;
    int mang[] = { 10, 20, 30, 40, 50, 25 };
    int n = sizeof(mang) / sizeof(mang[0]);

    // Chèn các phần tử vào cây AVL và in ra sau mỗi lần chèn
    printf("Chèn các phần tử và in cây sau mỗi bước:\n");
    for (int i = 0; i < n; i++) {
        printf("\nChèn: %d\n", mang[i]);
        goc = chen(goc, mang[i]);
        printf("Duyệt trước: ");
        duyetTruoc(goc);
        printf("\nDuyệt giữa: ");
        duyetGiua(goc);
        printf("\nDuyệt sau: ");
        duyetSau(goc);
        printf("\n-------------------\n");
    }

    printf("\nCây cuối cùng (Duyệt giữa): ");
    duyetGiua(goc);
    printf("\n");


    // Giải phóng bộ nhớ cấp cho cây.
    giaiPhongCay(goc);


    return 0;
}