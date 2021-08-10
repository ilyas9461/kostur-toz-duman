<h1 align="center">Koştur/Toz Duman/Araba Yarışı Oyun Makinesi </h1>


## İçerik

- [Giriş](#Giriş)
- [Çalışma Şekli](#Çalışma-Şekli)
- [Kullanılan Teknolojiler](#Kullanılan-Teknolojiler)
- [İletişim](#İletişim)

## Giriş

<p  align="center">
    <img src="./img/kostur_toz_duman3.png" width="70%" height="70%" border: 5px solid #555 >
</p>

 Bu oyun makinesi iki kişi tarafından oynanır. Sol tarafta 6 ve sağ tarafta 6 olmak üzere 12 tane göz bulunmaktadır. Oyuncuların karşısında her bir oyuncuyu ait makineye göre at, tavşan yada kaplumbağa veya araba bulunmaktadır. Oyun, oyuncuların karşısında atlar varsa at yarışı, tavşan ve kaplumbağa varsa toz-duman, arabalar varsa araba yarışı şeklinde oynanır ve makineye bu şekilde isim verilir. 
 

## Çalışma Şekli

Makine çalıştırıldığında yarıştırılacak nesneler en soldadır. Oyunculara toplar verilir. Bu toplar gözlerden içeri sokularak nesneler hareket ettirilir. Sokulan göze göre nesnenin aldığı yol farklıdır. 3 tane gözden herhangi birine top sokulduğunda bir birim yol alınıyorsa, tek gözde bu yol mktarı üç birimdir. Nesne yol aldığı sırada top herhangi bir göze tekrar sokulursa yol miktarı öncekinin üstüne eklenir. En sağa ilk varan oyunu kazanır. Aynı zamanda oyun oynama süresi bitiminde de oyun biter bu durumda önde olan kazanır.

<p  align="center">
    <img src="./pcb/kostur_pcb.png" width="90%" height="90%" border: 5px solid #555 >
</p

Oyun makinesinin bütün fonksiyonları mikrodenetleyici tabanlı bir ana kart ile kontrol edilmektedir. Oyun süresi, çalışması için gerekli olan jeton miktarı vb. ayarlar kart üzerindeki LCD den yapılmaktadır.

Oyun, oyun müziği eşliğinde oynanır. Toplar her geçtiğinde göze göre geçiş müziği çalar.

<br>
<p  align="center">
    <img src="./pcb/kostur_verici.png" width="80%" height="75%" border: 5px solid #555 >
     <img src="./pcb/kostur_alici.png" width="80%" height="75%" border: 5px solid #555 >
</p>

Topların geçişi iki adet infrared alıcı-verici devresi tarafından algılanarak ana karta uygun sinyal seviyesi oluşturulmaktadır. Programlamada gerçek zamnanlı bir algoritma gerçekleştirilerek top algılandığı anda, motora yol verilmiş, her delik için ayrı geçiş müziği oynatılmış, ilgili delik aydınlatılmıştır. 

Sensör çalışma videosu ...
[![](https://camo.githubusercontent.com/241d4106ff5edca2ee25e04dcf4546fad9d20b626f7a10990307e8f83e95459f/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f796f75747562652d2532334646303030302e7376673f267374796c653d666f722d7468652d6261646765266c6f676f3d796f7574756265266c6f676f436f6c6f723d7768697465253232)](https://youtu.be/8aHo4yhkyYs)


## Kullanılan Teknolojiler

```bash
- PIC18F45K22 mikrodenetleyici.
- CCS PIC C Derleyici
- Infrared Hareket algılama.
- Motor yön kontrolü.
- PCB devre şeması çizme ve oluşturma.

```

## Örnek çalışma videosu :

[![](https://camo.githubusercontent.com/241d4106ff5edca2ee25e04dcf4546fad9d20b626f7a10990307e8f83e95459f/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f796f75747562652d2532334646303030302e7376673f267374796c653d666f722d7468652d6261646765266c6f676f3d796f7574756265266c6f676f436f6c6f723d7768697465253232)](https://youtu.be/SfRwXPxYxrk)

## İletişim

- GitHub [@your-ilyas9461](https://github.com/ilyas9461)
- Linkedin [@your-linkedin](https://www.linkedin.com/in/ilyas-yağcioğlu-6a6b17217)

### *** Not : 
Devre endüstriyel olarak üretimde olduğu için PCB devre şemaları paylaşılmamıştır.
