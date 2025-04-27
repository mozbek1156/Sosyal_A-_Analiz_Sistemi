#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 1000
#define RED 0
#define BLACK 1

//  1- Kullanıcı yapısı
typedef struct User {
    int user_id;
    struct Friend* friends;  
    int friend_count;
} User;

// 2- Arkadaşlık yapısı (bağlı liste)
typedef struct Friend {
    struct User* user;
    struct Friend* next;
} Friend;

//  3- Kırmızı-Siyah ağaç düğümü
typedef struct RBNode {
    User* user;
    int color;  // 0: RED, 1: BLACK
    struct RBNode* parent;
    struct RBNode* left;
    struct RBNode* right;
} RBNode;

// 4- Sosyal ağ yapısı
typedef struct SocialNetwork {
    User* users[MAX_USERS];
    int user_count;
    RBNode* rb_root;  // Kullanıcı aramaları için Kırmızı-Siyah ağaç kökü
    RBNode* nil;      // Kırmızı-Siyah ağaç için NIL düğümü
} SocialNetwork;

//  5- Kullanıcı oluşturma
User* create_user(int user_id) {
    User* user = (User*)malloc(sizeof(User));
    if (user == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    user->user_id = user_id;
    user->friends = NULL;
    user->friend_count = 0;
    return user;
}

//  6- Arkadaşlık ekleme (graf yapısı)
void add_friendship(User* user1, User* user2) {
    // user1'e user2'yi ekle
    Friend* newFriend1 = (Friend*)malloc(sizeof(Friend));
    if (newFriend1 == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    newFriend1->user = user2;
    newFriend1->next = user1->friends;
    user1->friends = newFriend1;
    user1->friend_count++;
    
    // user2'ye user1'i ekle (çift yönlü graf)
    Friend* newFriend2 = (Friend*)malloc(sizeof(Friend));
    if (newFriend2 == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    newFriend2->user = user1;
    newFriend2->next = user2->friends;
    user2->friends = newFriend2;
    user2->friend_count++;
}

//  7- Kırmızı-Siyah ağaç için düğüm oluşturma
RBNode* create_rbnode(User* user, RBNode* nil) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    if (node == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    node->user = user;
    node->color = RED;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    return node;
}

//   8- Sol rotasyon
void left_rotate(SocialNetwork* network, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    
    if (y->left != network->nil)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == network->nil)
        network->rb_root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

//   8.1- Sağ rotasyon
void right_rotate(SocialNetwork* network, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    
    if (x->right != network->nil)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (y->parent == network->nil)
        network->rb_root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    
    x->right = y;
    y->parent = x;
}

//   9- Kırmızı-Siyah ağacına ekleme sonrası düzeltme
void rb_insert_fixup(SocialNetwork* network, RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(network, z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(network, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(network, z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(network, z->parent->parent);
            }
        }
        
        if (z == network->rb_root)
            break;
    }
    
    network->rb_root->color = BLACK;
}

//   10- Kırmızı-Siyah ağacına kullanıcı ekleme
void rb_insert(SocialNetwork* network, User* user) {
    RBNode* z = create_rbnode(user, network->nil);
    RBNode* y = network->nil;
    RBNode* x = network->rb_root;
    
    // BST ekleme işlemi
    while (x != network->nil) {
        y = x;
        if (z->user->user_id < x->user->user_id)
            x = x->left;
        else
            x = x->right;
    }
    
    z->parent = y;
    
    if (y == network->nil)
        network->rb_root = z;
    else if (z->user->user_id < y->user->user_id)
        y->left = z;
    else
        y->right = z;
    
    rb_insert_fixup(network, z);
}

//   11- Kırmızı-Siyah ağacında kullanıcı arama
User* rb_search(SocialNetwork* network, int user_id) {
    RBNode* current = network->rb_root;
    
    while (current != network->nil) {
        if (current->user->user_id == user_id)
            return current->user;
        
        if (user_id < current->user->user_id)
            current = current->left;
        else
            current = current->right;
    }
    
    return NULL;  // Kullanıcı bulunamadı
}

//   12- Sosyal ağ başlatma
SocialNetwork* init_social_network() {
    SocialNetwork* network = (SocialNetwork*)malloc(sizeof(SocialNetwork));
    if (network == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    
    network->user_count = 0;
    
    // Kırmızı-Siyah ağacı için NIL düğümü oluştur
    network->nil = (RBNode*)malloc(sizeof(RBNode));
    if (network->nil == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }
    
    network->nil->color = BLACK;
    network->nil->left = NULL;
    network->nil->right = NULL;
    network->nil->parent = NULL;
    network->nil->user = NULL;
    
    network->rb_root = network->nil;
    
    return network;
}

//   13- Sosyal ağa kullanıcı ekleme
void add_user(SocialNetwork* network, int user_id) {
    if (network->user_count >= MAX_USERS) {
        printf("Maksimum kullanici sayisina ulasildi!\n");
        return;
    }
    
    // Önce kullanıcının zaten var olup olmadığını kontrol et
    if (rb_search(network, user_id) != NULL) {
        printf("User %d zaten mevcut!\n", user_id);
        return;
    }
    
    User* user = create_user(user_id);
    network->users[network->user_count++] = user;
    
    // Kullanıcıyı Kırmızı-Siyah ağacına ekle
    rb_insert(network, user);
    
    printf("User %d eklendi.\n", user_id);
}

//  14- DFS ile belirli mesafedeki arkadaşları bulma
void find_friends_at_distance_dfs(User* start, int current_dist, int target_dist, int* visited, User** result, int* result_count) {
    // Kullanıcıyı ziyaret edildi olarak işaretle
    visited[start->user_id] = 1;
    
    if (current_dist == target_dist) {
        // Hedef mesafeye ulaşıldı, kullanıcıyı sonuç listesine ekle
        result[*result_count] = start;
        (*result_count)++;
        return;
    }
    
    // Tüm arkadaşları ziyaret et
    Friend* current = start->friends;
    while (current != NULL) {
        if (!visited[current->user->user_id]) {
            find_friends_at_distance_dfs(current->user, current_dist + 1, target_dist, visited, result, result_count);
        }
        current = current->next;
    }
}

//  15- Belirli mesafedeki arkadaşları bulma
User** find_friends_at_distance(SocialNetwork* network, int user_id, int distance, int* count) {
    User* user = rb_search(network, user_id);
    if (user == NULL) {
        printf("Kullanici bulunamadi: %d\n", user_id);
        *count = 0;
        return NULL;
    }
    
    // Ziyaret edilen kullanıcıları takip etmek için dizi
    int* visited = (int*)calloc(MAX_USERS, sizeof(int));
    
    // Sonuç için kullanıcı dizisi
    User** result = (User**)malloc(MAX_USERS * sizeof(User*));
    *count = 0;
    
    find_friends_at_distance_dfs(user, 0, distance, visited, result, count);
    
    free(visited);
    return result;
}

//  16- Ortak arkadaş analizi
int* find_common_friends(SocialNetwork* network, int user_id1, int user_id2, int* count) {
    User* user1 = rb_search(network, user_id1);
    User* user2 = rb_search(network, user_id2);
    
    if (user1 == NULL || user2 == NULL) {
        printf("Kullanicilardan biri veya her ikisi bulunamadi!\n");
        *count = 0;
        return NULL;
    }
    
    // Kullanıcı 1'in arkadaşları için hash tablosu
    int* user1_friends = (int*)calloc(MAX_USERS, sizeof(int));
    
    // Kullanıcı 1'in tüm arkadaşlarını işaretle
    Friend* current = user1->friends;
    while (current != NULL) {
        user1_friends[current->user->user_id] = 1;
        current = current->next;
    }
    
    // Ortak arkadaşları bul
    int* common_friends = (int*)malloc(MAX_USERS * sizeof(int));
    *count = 0;
    
    current = user2->friends;
    while (current != NULL) {
        if (user1_friends[current->user->user_id]) {
            common_friends[*count] = current->user->user_id;
            (*count)++;
        }
        current = current->next;
    }
    
    free(user1_friends);
    return common_friends;
}

//  17- Etki alanı hesaplama (bir kullanıcının ağ üzerindeki etkisi)
int calculate_influence(SocialNetwork* network, int user_id, int max_distance) {
    User* user = rb_search(network, user_id);
    if (user == NULL) {
        printf("Kullanici bulunamadi: %d\n", user_id);
        return 0;
    }
    
    int influence = 0;
    
    // Her mesafe için etki alanını hesapla
    for (int dist = 1; dist <= max_distance; dist++) {
        int count;
        User** users_at_dist = find_friends_at_distance(network, user_id, dist, &count);
        
        // Her mesafedeki kullanıcılar için ağırlık belirleme (mesafe arttıkça azalan etki)
        double weight = 1.0 / (double)dist;
        influence += (int)(count * weight);
        
        free(users_at_dist);
    }
    
    return influence;
}

//  18- Topluluk tespiti - basit bir yaklaşım
void detect_communities(SocialNetwork* network) {
    // Bu basit bir topluluk tespit algoritmasıdır
    // Gerçek uygulamalarda daha karmaşık algoritmalar kullanılır
    
    // Ziyaret edilen kullanıcıları takip etmek için dizi
    int* visited = (int*)calloc(MAX_USERS, sizeof(int));
    int community_id = 1;
    
    for (int i = 0; i < network->user_count; i++) {
        User* user = network->users[i];
        
        if (!visited[user->user_id]) {
            printf("Topluluk %d:\n", community_id);
            
            // BFS ile topluluk üyelerini bul
            User** queue = (User**)malloc(MAX_USERS * sizeof(User*));
            int front = 0, rear = 0;
            
            queue[rear++] = user;
            visited[user->user_id] = community_id;
            
            while (front < rear) {
                User* current_user = queue[front++];
                printf("  USER %d\n", current_user->user_id);
                
                Friend* friend_ptr = current_user->friends;
                while (friend_ptr != NULL) {
                    if (!visited[friend_ptr->user->user_id]) {
                        queue[rear++] = friend_ptr->user;
                        visited[friend_ptr->user->user_id] = community_id;
                    }
                    friend_ptr = friend_ptr->next;
                }
            }
            
            free(queue);
            community_id++;
        }
    }
    
    free(visited);
}

//  19- Örnek verileri yükleme
void load_sample_data(SocialNetwork* network) {
    // Kullanıcıları ekle
    add_user(network, 101);
    add_user(network, 102);
    add_user(network, 103);
    add_user(network, 104);
    add_user(network, 105);
    
    // Arkadaşlık ilişkilerini ekle
    User* user101 = rb_search(network, 101);
    User* user102 = rb_search(network, 102);
    User* user103 = rb_search(network, 103);
    User* user104 = rb_search(network, 104);
    User* user105 = rb_search(network, 105);
    
    add_friendship(user101, user102);
    add_friendship(user101, user103);
    add_friendship(user102, user104);
    add_friendship(user103, user104);
    add_friendship(user104, user105);
}

// 20-  Ana uygulama
int main() {
    SocialNetwork* network = init_social_network();
    
    printf("Sosyal Ag Analiz Sistemi\n");
    printf("------------------------\n\n");
    
    load_sample_data(network);
    
    // 1. Örnek: Belirli bir mesafedeki arkadaşları bulma
    printf("\n1. Belirli mesafedeki arkadaslari bulma:\n");
    int count;
    User** distance_2_friends = find_friends_at_distance(network, 101, 2, &count);
    printf("USER 101'in 2 mesafedeki arkadaslari (%d kisi):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  USER %d\n", distance_2_friends[i]->user_id);
    }
    free(distance_2_friends);
    
    // 2. Örnek: Ortak arkadaş analizi
    printf("\n2. Ortak arkadas analizi:\n");
    int* common = find_common_friends(network, 102, 103, &count);
    printf("USER 102 ve USER 103'un ortak arkadaslari (%d kisi):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  USER %d\n", common[i]);
    }
    free(common);
    
    // 3. Örnek: Etki alanı hesaplaması
    printf("\n3. Etki alani hesaplamasi:\n");
    int influence = calculate_influence(network, 104, 2);
    printf("USER 104'un etki puani: %d\n", influence);
    
    // 4. Örnek: Topluluk tespiti
    printf("\n4. Topluluk tespiti:\n");
    detect_communities(network);
    
    
    
    return 0;
}