#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 10000
#define TABLE_SIZE 1000


typedef struct Device {
    int id;
    char name[50];
    char category[50];
    int usageLevel;  
    float value;     
    struct Device* next; 
} Device;


typedef struct BorrowRecord {
    int deviceId;
    char borrowerName[50];
    int isReturned; // 0 = dang muon, 1 = da tra
    struct BorrowRecord* next;
} BorrowRecord;


Device* hashTable[TABLE_SIZE]; 
Device* deviceArray[MAX_DEVICES]; // Mang luu thiet bi de duyet va sap xep
int deviceCount = 0;
BorrowRecord* borrowList = NULL; 

// Ham tinh gia tri bam
int hashFunction(int id) {
    return id % TABLE_SIZE;
}


// Tim thiet bi theo ID
Device* findDevice(int id) {
    int index = hashFunction(id);
    Device* current = hashTable[index];
    while (current != NULL) {
        if (current->id == id) {
            // Hi?n th? thông tin thi?t b? trong khung
            printf("\nThiet bi tim thay:\n");
            printf("=======================================================================================\n");
            printf("| %-10s | %-20s | %-20s | %-15s | %-10s |\n", 
                   "ID", "Ten", "Loai", "Muc do su dung", "Gia tri");
            printf("=======================================================================================\n");
            printf("| %-10d | %-20s | %-20s | %-15d | %-10.2f |\n", 
                   current->id, current->name, current->category, current->usageLevel, current->value);
            printf("=======================================================================================\n");
            return current;
        }
        current = current->next;
    }
    printf("Khong tim thay thiet bi ID %d\n", id);
    return NULL;
}

int isDuplicateId(int id) {
    return findDevice(id) != NULL;
}
// Them thiet bi
void addDevice(int id, const char* name, const char* category, int usageLevel, float value) {
	
	if (isDuplicateId(id)) {
        printf("ID %d da ton tai, khong the them thiet bi moi!\n", id);
        return;
    }
    
    int index = hashFunction(id);
    Device* newDevice = (Device*)malloc(sizeof(Device));
    newDevice->id = id;
    strcpy(newDevice->name, name);
    strcpy(newDevice->category, category);
    newDevice->usageLevel = usageLevel;
    newDevice->value = value;
    newDevice->next = hashTable[index];
    hashTable[index] = newDevice;

    deviceArray[deviceCount++] = newDevice;
     printf("Da them thiet bi: %s\n", name);
}


// Xoa thiet bi theo ID
void removeDevice(int id) {
    int index = hashFunction(id);
    Device* current = hashTable[index];
    Device* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }

            // Xoa khoi mang
            for (int i = 0; i < deviceCount; i++) {
                if (deviceArray[i]->id == id) {
                    deviceArray[i] = deviceArray[--deviceCount];
                    break;
                }
            }

            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Khong tim thay thiet bi: %d\n", id);
}

// Cap nhat thong tin thiet bi
void updateDevice(int id, const char* name, const char* category, int usageLevel, float value) {
    Device* device = findDevice(id);
    if (device) {
        if (name) strcpy(device->name, name);
        if (category) strcpy(device->category, category);
        device->usageLevel = usageLevel;
        device->value = value;
        printf("Da cap nhat thiet bi: %d\n", id);
    } else {
        printf("Khong tim thay thiet bi: %d\n", id);
    }
}

// Hien thi danh sach thiet bi
void displayDevices() {
    if (deviceCount == 0) {
        printf("Danh sach thiet bi rong.\n");
        return;
    }

    printf("=======================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-10s |\n", 
           "ID", "Ten", "Loai", "Muc do su dung", "Gia tri");
    printf("=======================================================================================\n");

    for (int i = 0; i < deviceCount; i++) {
        Device* current = deviceArray[i];
        printf("| %-10d | %-20s | %-20s | %-15d | %-10.2f |\n",
               current->id, current->name, current->category, current->usageLevel, current->value);
    }

    printf("=======================================================================================\n");
}


// Muon thiet bi
void borrowDevice(int deviceId, char* borrowerName) {
    BorrowRecord* record = (BorrowRecord*)malloc(sizeof(BorrowRecord));
    record->deviceId = deviceId;
    strcpy(record->borrowerName, borrowerName);
    record->isReturned = 0;
    record->next = borrowList;
    borrowList = record;
    printf("Thiet bi %d da duoc muon boi: %s\n", deviceId, borrowerName);
}

// Tra thiet bi
void returnDevice(int deviceId) {
    BorrowRecord* current = borrowList;
    while (current) {
        if (current->deviceId == deviceId && current->isReturned == 0) {
            current->isReturned = 1;
            printf("Thiet bi %d da duoc tra\n", deviceId);
            return;
        }
        current = current->next;
    }
    printf("Thiet bi %d khong co trong danh sach muon\n", deviceId);
}

// Hien thi danh sach muon/tra
void displayBorrowRecords() {
    if (!borrowList) {
        printf("Danh sach muon/tra rong.\n");
        return;
    }

    printf("===============================================================================\n");
    printf("| %-10s | %-20s | %-15s |\n", "ID Thiet bi", "Nguoi muon", "Trang thai");
    printf("===============================================================================\n");

    BorrowRecord* current = borrowList;
    while (current) {
        printf("| %-10d | %-20s | %-15s |\n", 
               current->deviceId, current->borrowerName, current->isReturned ? "Da tra" : "Dang muon");
        current = current->next;
    }

    printf("===============================================================================\n");
}


void heapify(int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && deviceArray[left]->value > deviceArray[largest]->value) {
        largest = left;
    }

    if (right < n && deviceArray[right]->value > deviceArray[largest]->value) {
        largest = right;
    }

    if (largest != i) {
        Device* temp = deviceArray[i];
        deviceArray[i] = deviceArray[largest];
        deviceArray[largest] = temp;

        heapify(n, largest);
    }
}

void heapify1(int n, int i) {
	int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && deviceArray[left]->usageLevel > deviceArray[largest]->usageLevel) {
        largest = left;
    }

    if (right < n && deviceArray[right]->usageLevel > deviceArray[largest]->usageLevel) {
        largest = right;
    }

    if (largest != i) {
        Device* temp = deviceArray[i];
        deviceArray[i] = deviceArray[largest];
        deviceArray[largest] = temp;

        heapify1(n, largest);
    }
}

void heapSortDevicesByUsage() {
    for (int i = deviceCount / 2 - 1; i >= 0; i--) {
        heapify1(deviceCount, i);
    }

    for (int i = deviceCount - 1; i >= 0; i--) {
        Device* temp = deviceArray[0];
        deviceArray[0] = deviceArray[i];
        deviceArray[i] = temp;

        heapify1(i, 0);
    }
}
	

void heapSortDevices() {
    for (int i = deviceCount / 2 - 1; i >= 0; i--) {
        heapify(deviceCount, i);
    }

    for (int i = deviceCount - 1; i >= 0; i--) {
        Device* temp = deviceArray[0];
        deviceArray[0] = deviceArray[i];
        deviceArray[i] = temp;

        heapify(i, 0);
    }
}

void sortDevicesByValue() {
    heapSortDevices();
    printf("\nThiet thi sap xep theo gia tri:\n");
    displayDevices();
}

void sortDevicesByUsageLevel() {
    heapSortDevicesByUsage();
    printf("\nThiet bi sap xep theo muc do su dung:\n");
    displayDevices();
}

int main() {
    int choice, id, usageLevel;
    char name[50],nameborrow[50] ,category[50];
    float value;
    

    while (1) {
        printf("\nQuan li thiet bi phong lab:\n");
        printf("1. Them thiet bi\n");
        printf("2. Hien thi danh sach thiet bi\n");
        printf("3. Muon thiet bi\n");
        printf("4. Tra thiet bi\n");
        printf("5. Xoa thiet bi\n");
        printf("6. Tim kiem thiet bi theo id\n");
        printf("7. Sap xep theo gia tri\n");
        printf("8, Sap xe theo muc do su dung\n");
        printf("9, Hien thi danh sach muon tra\n");
        printf("10. Thoat\n");
        printf("Lua chon: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("Nhap ID, ten, loai, muc su dung, gia tri: \n");
            printf("ID: ");
            scanf("%d", &id);
            getchar();
            printf("Ten: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Loai: ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = '\0';
            printf("Muc su dung: ");
            scanf("%d", &usageLevel);
            printf("Gia tri: ");
            scanf("%f", &value);
            
            addDevice(id, name, category,usageLevel, value );
            break;

        case 2:
            displayDevices();
            break;

        case 3:
            printf("Nhap ID thiet bi muon: ");
            scanf("%d", &id);
            getchar();
            printf("Ten: ");
            fgets(nameborrow, sizeof(nameborrow), stdin);
            nameborrow[strcspn(nameborrow, "\n")] = '\0';
            borrowDevice(id, nameborrow);
            break;

        case 4:
            printf("Nhap ID thiet bi tra: ");
            scanf("%d", &id);
            returnDevice(id);
            break;

        case 5:
            printf("Nhap ID thiet bi can xoa: ");
            scanf("%d", &id);
            removeDevice(id);
            break;
        case 6:
             printf("Nhap id thiet bi can tim: ");
             scanf("%d", &id);
             findDevice(id);
            break;

        case 7:
        	sortDevicesByValue();
        	break;
        case 8:
            sortDevicesByUsageLevel();
            break;
        case 9:
        	displayBorrowRecords();
        	break;
        case 10:
            printf("Thoat chuong trinh!\n");
            return 0;

        default:
            printf("Lua chon khong hop le!\n");
        }
    }
    


    return 0;
}




