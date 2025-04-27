# Sosyal Ağ Analiz Sistemi

Bu C programı, sosyal ağ grafiğini ağaç tabanlı veri yapıları kullanarak modelleyen ve çeşitli sosyal ağ analizleri gerçekleştiren bir sistemdir.

## Genel Bakış

Sistem, kullanıcılar arasındaki ilişkileri graf olarak temsil eder ve analiz için özellikle ağaç veri yapılarını kullanır. Dengeli ve verimli kullanıcı aramaları için Kırmızı-Siyah ağaç veri yapısı kullanılmaktadır.

## Desteklenen Özellikler

1. **İlişki Ağacı Oluşturma**: Kullanıcılar ve aralarındaki arkadaşlık ilişkileri temsil edilir.
2. **Derinlik-İlk Arama (DFS)**: Belirli mesafedeki arkadaşları bulma işlemi DFS algoritması ile gerçekleştirilir.
3. **Ortak Arkadaş Analizi**: İki kullanıcı arasındaki ortak arkadaşları tespit eder.
4. **Topluluk Tespiti**: Birbirine bağlı kullanıcı gruplarını tespit eder.
5. **Etki Alanı Hesaplama**: Bir kullanıcının ağ üzerindeki etkisini hesaplar.
6. **Kırmızı-Siyah Ağaç Araması**: Kullanıcı aramaları için dengeli bir ağaç yapısı kullanılır.

## Veri Yapıları

Program aşağıdaki temel veri yapılarını kullanır:

1. **User**: Kullanıcı bilgilerini saklayan yapı

   - `user_id`: Benzersiz kullanıcı kimliği
   - `friends`: Arkadaşlık ilişkilerini tutan bağlı liste başlangıcı
   - `friend_count`: Toplam arkadaş sayısı

2. **Friend**: Arkadaşlık ilişkilerini bağlı liste olarak tutan yapı

   - `user`: Arkadaş olan kullanıcının referansı
   - `next`: Listede bir sonraki arkadaşın referansı

3. **RBNode**: Kırmızı-Siyah ağaç düğümü

   - `user`: Düğümde saklanan kullanıcı verisi
   - `color`: Düğümün rengi (RED veya BLACK)
   - `parent`, `left`, `right`: Ağaç yapısı için bağlantılar

4. **SocialNetwork**: Tüm sosyal ağı yöneten ana yapı
   - `users`: Sistemdeki tüm kullanıcıların dizisi
   - `user_count`: Toplam kullanıcı sayısı
   - `rb_root`: Kırmızı-siyah ağacın kök düğümü
   - `nil`: Kırmızı-siyah ağaç için özel NIL düğümü

## Temel Algoritmalar

Program aşağıdaki temel algoritmaları kullanır:

1. **DFS (Depth-First Search)**: Belirli mesafedeki arkadaşları bulmak için kullanılır.
2. **BFS (Breadth-First Search)**: Topluluk tespiti için kullanılır.
3. **Kırmızı-Siyah Ağaç**: Dengeli bir ikili arama ağacı yapısıdır. Kullanıcı aramalarını O(log n) zamanda gerçekleştirir.

## Kullanım

Program, bir sosyal ağ analizi sistemi olarak test verilerini kullanarak çeşitli analiz örnekleri sunar:

1. Belirli mesafedeki arkadaşları bulma
2. Ortak arkadaş analizi
3. Etki alanı hesaplaması
4. Topluluk tespiti

## Örnek Veri Seti

Program aşağıdaki örnek veriyle çalışır:

```
# Kullanıcılar
USER 101
USER 102
USER 103
USER 104
USER 105

# Arkadaşlık ilişkileri
FRIEND 101 102
FRIEND 101 103
FRIEND 102 104
FRIEND 103 104
FRIEND 104 105
```

## Derleme ve Çalıştırma

Programı derlemek için:

```bash
gcc -o social_network social_network.c
```

Programı çalıştırmak için:

```bash
./social_network
```

## Fonksiyonların Özeti

1. **create_user()**: Yeni bir kullanıcı oluşturur
2. **add_friendship()**: İki kullanıcı arasında arkadaşlık ilişkisi kurar
3. **create_rbnode()**: Kırmızı-siyah ağaç için yeni bir düğüm oluşturur
4. **left_rotate()**, **right_rotate()**: Kırmızı-siyah ağaç rotasyon işlemleri
5. **rb_insert()**: Kırmızı-siyah ağaca kullanıcı ekler
6. **rb_search()**: Kırmızı-siyah ağaçta kullanıcı arar
7. **init_social_network()**: Sosyal ağ yapısını başlatır
8. **add_user()**: Sosyal ağa kullanıcı ekler
9. **find_friends_at_distance_dfs()**: DFS ile belirli mesafedeki arkadaşları bulur
10. **find_common_friends()**: İki kullanıcının ortak arkadaşlarını bulur
11. **calculate_influence()**: Bir kullanıcının ağ üzerindeki etkisini hesaplar
12. **detect_communities()**: Birbirine bağlı kullanıcı gruplarını tespit eder

## Notlar ve Geliştirme Alanları

- Program bellek yönetimi için dinamik bellek ayırma kullanır. Gerçek uygulamalarda tüm belleklerin düzgün şekilde serbest bırakıldığından emin olunmalıdır.
- Topluluk tespiti algoritması basit bir BFS tabanlı yaklaşım kullanır. Gerçek uygulamalarda daha karmaşık algoritmalar (Louvain, Girvan-Newman vb.) kullanılabilir.
- Programın ölçeklenebilirliği için MAX_USERS ve ilgili veri yapıları gerektiğinde düzenlenebilir.
- Gerçek kullanım senaryolarına göre işlevsellik eklenebilir.
