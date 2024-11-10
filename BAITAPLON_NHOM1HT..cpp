#include <iostream>
#include <cstring>
#include <limits>
#include <stack>

using namespace std;

#define HANG 5 
#define COT 5  

typedef struct khach {
    char Ten[30];
    int STT;
    char ViTri[10];
    double SoTien;
    char gioVao[10];  
    khach* next; 
} khach;

stack<khach*> luuStack;

typedef struct list {
    khach* head;
    khach* tail;
    int size;
    char bangGhe[HANG][COT]; 
    double doanhThu;
    
    list();
    ~list();
    khach* createNode();
    void addfirst();
    void addlast();
    void deleteFirst(); 
    void xoaKhachTheoYeuCau();
    void updateSTT(); 
    void insert(int pos);
    void hienThiDanhSachKhach();
    bool datGhe(int& hang, int& cot);  
	void hienThiBangGhe();  
    void hienThiSoLuongGhe(); 
    void hienThiDoanhThu();
    void xuatHoaDon(); 
    void deleteAll();
    void phucHoiVe(); 
} list;

list::list() {
    head = tail = NULL;
    size = 0;
    doanhThu = 0.0;  
    for (int i = 0; i < HANG; i++) {
        for (int j = 0; j < COT; j++) {
            bangGhe[i][j] = '-';
        }
    }
}

list::~list() {
    deleteAll();
}

khach* list::createNode() {
    khach* p = new khach();
    cout << "Nhap ten : ";
    cin.ignore();
    cin.getline(p->Ten, sizeof(p->Ten));

    int hang, cot;
    if (!datGhe(hang, cot)) {
        cout << "Khong the dat ghe! Vui long thu lai.\n";
        delete p;  
        return NULL;
    }

    sprintf(p->ViTri, "H%c-C%d", hang + 'A', cot + 1);

    if (hang < 2) {
        p->SoTien = 45000;  
    } else {
        p->SoTien = 55000;  
    }

    doanhThu += p->SoTien;

    cout << "Nhap gio vao rap (hh:mm): ";
    cin.ignore();  
    cin.getline(p->gioVao, sizeof(p->gioVao));

    int hour, minute;
    if (sscanf(p->gioVao, "%d:%d", &hour, &minute) != 2 || hour < 0 || hour >= 24 || minute < 0 || minute >= 60) {
        cout << "Gio vao rap khong hop le. Vui long nhap lai.\n";
        cout << "Nhap gio vao rap (hh:mm): ";
        cin.getline(p->gioVao, sizeof(p->gioVao));
    }

    p->next = NULL;
    return p;
}

void list::addfirst() {
    khach* v = createNode();
    if (v == NULL) return;  

    v->next = head;
    head = v;
    if (tail == NULL) {
        tail = head;
    }
    size++;
    updateSTT();
}

void list::addlast() {
    khach* v = createNode();
    if (v == NULL) return; 

    if (head == NULL) {
        head = tail = v;
    } else {
        tail->next = v;
        tail = v;
    }
    size++;
    updateSTT();
}
void list::deleteFirst() {
    if (head == NULL) {
        cout << "Danh sach khong co khach de xoa!" << endl;
        return;
    }

    khach* temp = head;
    head = head->next;
    doanhThu -= temp->SoTien;

    if (head == NULL) {
        tail = NULL;
    }

    luuStack.push(temp);  
    size--;
    updateSTT();
    cout << "Da xoa khach dau tien trong danh sach va luu vao stack." << endl;
}

void list::updateSTT() {
    khach* current = head;
    int stt = 1;
    while (current != NULL) {
        current->STT = stt;
        stt++;
        current = current->next;
    }
}
void list::xoaKhachTheoYeuCau() {
    if (head == NULL) {
        cout << "Danh sach khach rong!" << endl;
        return;
    }

    char viTri[10];
    cout << "Nhap vi tri khach muon xoa (VD: HA-C1): ";
    cin >> viTri;

    khach* current = head;
    khach* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->ViTri, viTri) == 0) {
            int hang = current->ViTri[1] - 'A';
            int cot = current->ViTri[3] - '1';
            bangGhe[hang][cot] = '-';

            doanhThu -= current->SoTien;

            if (prev == NULL) {
                head = current->next;
                if (head == NULL) {
                    tail = NULL;
                }
            } else {
                prev->next = current->next;
                if (current == tail) {
                    tail = prev;
                }
            }

            luuStack.push(current); 
            size--;
            updateSTT();
            cout << "Da xoa khach tai vi tri " << viTri << " va luu vao stack." << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Khach khong tim thay!" << endl;
}
void list::phucHoiVe() {
    if (luuStack.empty()) {
        cout << "Khong co ve nao de phuc hoi!" << endl;
        return;
    }

    khach* phuchoi = luuStack.top();
    luuStack.pop();

   
    int hang = phuchoi->ViTri[1] - 'A';
    int cot = phuchoi->ViTri[3] - '1';
    bangGhe[hang][cot] = 'X';

   
    if (head == NULL) {
        head = tail = phuchoi;
    } else {
        tail->next = phuchoi;
        tail = phuchoi;
    }
    tail->next = NULL;

    doanhThu += phuchoi->SoTien;
    size++;
    updateSTT();

    cout << "Da phuc hoi ve cho khach " << phuchoi->Ten << " tai vi tri " << phuchoi->ViTri << endl;
}
void list::insert(int pos) {
    if (pos < 0 || pos > size) { 
        cout << "Vi tri chen khong hop le!" << endl;
        return;
    }

    khach* kh = createNode();   

    if (pos == 0) {             
        kh->next = head;         
        head = kh;              
        if (size == 0) {        
            tail = kh;          
        }
    } else if (pos == size) {    
        tail->next = kh;      
        tail = kh;               
    } else {                    
        khach* p = head;      
        for (int i = 0; i < pos - 1; i++) { 
            p = p->next;         
        }
        kh->next = p->next;    
        p->next = kh;        
    }

    size++;                     
    updateSTT();               
}
void list::deleteAll() {
    if (head == NULL) {
        cout << "Danh sach da rong" << endl;
        return;
    }

    khach* p;
    while (head != NULL) {
        p = head;
        head = head->next;
        delete p;
    }
    tail = NULL;

    for (int i = 0; i < HANG; i++) {
        for (int j = 0; j < COT; j++) {
            bangGhe[i][j] = '-';
        }
    }
    doanhThu = 0.0;  
}
bool list::datGhe(int& hang, int& cot) {
    char rowChar;
    cout << "Nhap hang (A-E)(Luu y: Tu hang A->B Gia ve 45000VND C->E Gia ve 55000VND): ";
    cin >> rowChar;
    hang = rowChar - 'A'; 

    cout << "Nhap cot (1-" << COT << "): ";
    cin >> cot;
    cot--;  

    while (hang < 0 || hang >= HANG || cot < 0 || cot >= COT || bangGhe[hang][cot] != '-') {
        cout << "Vi tri khong hop le hoac ghe da duoc dat. Vui long nhap lai.\n";
        cout << "Nhap hang (A-E): ";
        cin >> rowChar;
        hang = rowChar - 'A';

        cout << "Nhap cot (1-" << COT << "): ";
        cin >> cot;
        cot--;
    }

    bangGhe[hang][cot] = 'X'; 
    return true;
}

void list::hienThiBangGhe() {
    cout << "\nBang ghe:\n";
    cout << "   ";
    for (int j = 1; j <= COT; j++) {
        cout << j << " ";  
    }
    cout << endl;
    for (int i = 0; i < HANG; i++) {
        cout << static_cast<char>('A' + i) << " "; 
        for (int j = 0; j < COT; j++) {
            cout << bangGhe[i][j] << " ";
        }
        cout << endl;
    }
}

void list::hienThiSoLuongGhe() {
    int emptySeats = 0;
    for (int i = 0; i < HANG; i++) {
        for (int j = 0; j < COT; j++) {
            if (bangGhe[i][j] == '-') {
                emptySeats++;
            }
        }
    }
    cout << "So luong ghe trong: " << emptySeats << endl;
}

void list::hienThiDoanhThu() {
    cout << "Doanh thu hien tai: " << doanhThu << " VND" << endl;
}

void list::xuatHoaDon() {
    if (head == NULL) {
        cout << "Danh sach khach rong!" << endl;
        return;
    }

    khach* p = head;
    while (p != NULL) {
        cout << "Hoa don cho khach: " << p->Ten << endl;
        cout << "Vi tri ghe: " << p->ViTri << endl;
        cout << "Gio vao rap: " << p->gioVao << endl;
        cout << "So tien thanh toan: " << p->SoTien << " VND" << endl;
        cout << "-----------------------------\n";
        p = p->next;
    }
}
void list::hienThiDanhSachKhach() {
    cout << "\n-------------------------------DANH SACH KHACH HANG--------------------------------\n";
    cout << "|  STT   |       Ten khach hang      |  So ghe    |  Gio vao rap |   Gia ve         |\n";
    cout << "-----------------------------------------------------------------------------------\n";

    khach* p = head;
    while (p != NULL) {
        
        printf("| %-6d | %-25s | %-10s | %-12s | %-12.2f VND |\n", 
               p->STT, p->Ten, p->ViTri, p->gioVao, p->SoTien);
        p = p->next;
    }

    cout << "-----------------------------------------------------------------------------------\n";
}
int main() {
    list l;
    int choice, pos;

    do {
        cout << "\n-----------Menu-----------\n";
        cout << "1. Nhap thong tin dat ghe\n";
        cout << "2. Them khach vao dau danh sach\n";
        cout << "3. Chen khach vao vi tri\n";
        cout << "4. Xoa khach dau tien\n";  
        cout << "5. Xoa khach theo yeu cau\n";
        cout << "6. Xoa tat ca khach\n";
        cout << "7. Phuc hoi ve vua xoa\n";
        cout << "8. Hien thi danh sach khach\n";
        cout << "9. Hien thi bang ghe\n";
        cout << "10. Hien thi so luong ghe\n";
        cout << "11. Hien thi doanh thu\n";
        cout << "12. Xuat hoa don\n";
        cout << "13. Thoat\n";
        cout << "Nhap yeu cau: ";
        cin >> choice;

        switch (choice) {
            case 1:
                {
                    int numKhach;
                    cout << "Nhap so luong khach muon them: ";
                    cin >> numKhach;

                    for (int i = 0; i < numKhach; i++) {
                        cout << "\nNhap thong tin cho khach " << i + 1 << ":\n";
                        l.addlast();  
                    }
                }
                break;
            case 2:
                l.addfirst();
                break;
            case 3:
                cout << "Nhap vi tri khach muon chen: ";
                cin >> pos;
                l.insert(pos);
                break;
            case 4:
                l.deleteFirst();
                break;
            case 5:
                l.xoaKhachTheoYeuCau();
                break;
            case 6:
                l.deleteAll();  
                break;
            case 7:
                l.phucHoiVe();
                break;
            case 8:
                l.hienThiDanhSachKhach();
                break;
            case 9:
                l.hienThiBangGhe();
                break;
            case 10:
                l.hienThiSoLuongGhe();
                break;
            case 11:
                l.hienThiDoanhThu();
                break;
            case 12:
                l.xuatHoaDon();
                break;
                break;
            case 13:
                cout << "Thoat chuong trinh. Cam on!" << endl;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long chon lai!" << endl;
        }
    } while (choice != 13);

    return 0;
}

