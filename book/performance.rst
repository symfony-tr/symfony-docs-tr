.. index::
   single: Testler

Performans
===========

İlk izlenim olarak Symfony2 hızlıdır. Elbette eğer gerçekten hıza ihtiyacınız
var ise Symfony'yi gerçekten hızlı yapmak için pek çok yolunuz var. Bu 
kısımda Smyonfy uygulamasını gerçekten hızlı yapmak için en sık ve 
en güçlü yolları keşfedeceksiniz.

.. index::
   single: Performans; Byte code ön belleklemesi

Byte Code Ön Belleklemesi Kullanımı (Örn: APC)
----------------------------------------------

En iyi (ve en kolay) şey, "byte code cache" ile performansınızı arttırabilmenizdir.
Byte Code önbellieklemesinin düşüncesi sık sık PHP kodunun sürekli derlenmesinin
önüne geçilmesidir. Pek çok `byte code ön bellekleme`_  araçları bulunmaktadır.
Bunların bazıları da açık kaynak kodludur. En çok kullanılan byte code ön belleklemesi
muhtemelen `APC`_ dir.
Byte code ön belleklemesi kullanmanın herhangi bir dezavantajı yoktur ve Symfony2
bu tip bir ortamda en iyi çalışacak şekilde tasarlanmıştır.

Daha Fazla Optimizasyon
~~~~~~~~~~~~~~~~~~~~~~~~

Byte code ön belleklemesi genellikle değişiklikler için kaynak dosyaları 
izler.Bu kaynak dosya değişikliklerinde, byte kodu otomatik olarak yeniden
derlenerek sağlanır. Bu oldukça elverişli ancak bir sürü iş bindiren bir
işlemdir. 

Bu yüzden bazı byte code ön belleklemesi bu değişiklikleri kontrol etme
mekanizmasını devreden çıkaracak bazı sistemler ile donatılmıştır. 
Bu kontroller devre dışı bırakıldığında kaynak dosya değişikliklerini
kontrol etme işi sunucu yönetcisinine kalır ve sunucu yöneticisi her dosya
içeriği deişilkiğinde ön belleği temizlemesi gerekir. Aksi takdirde
güncellemeler gözükmeyecektir.

Örneğin, APC'de kontrollerin devre dışı bırakılması için php.ini
konfigürasyonuna ``apc.start=0`` satırının eklenmesi gerekir.

.. index::
   single: Performans; Autoloader

Bu ön belleklerde Autoloader kullanmak (Örn. ``ApcUniversalClassLoader``)
-------------------------------------------------------------------------

Varsayılan olarak Symfony2 standart sürüm de `autoloader.php`_ 
dosyasında ``UniversalClassLoader`` kullanır. Bu autoloader kolay
bir şekilde otomatik olarak yeni sınıfları verilen dizinler içerisinde bulur.

Malesef bu, loader tüm konfigüre edilmiş namespace'leri kapsayan dosyaları 
bulması ve gerçekten bu dosyaları ``file_exist`` cağrısı yapana kadar
araması gibi ekstra bir yük getirir.

En basit çözüm her sınıfın yerini sınıf ilk kez yüklendikten sonra ön belleğe
almaktır. Symfony sınıf konumlarını APC içerisinde  saklayan ``UniversalClassLoader``
dan türetimiş bir ``ApcUniversalClassLoader`` sınıfı ile birlikte gelir.

Bu snıf yükleyicisini kullanmak için basitçe ``autoloader.php`` dosyasına
şu şekilde adapte edin:

.. code-block:: php

    // app/autoload.php
    require __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('some caching unique prefix');
    // ...

.. note::

    APC autoloader'ini kullanırken eğer yeni sınıf eklediyseniz bu sınıf
    otomatik olarak bulunur ve herşey aynı önceki gibi çalışır(yani 
    ön belleği temizlemek için bir sebep yoktur). Ancak, eğer namespace'in
    bir kısmını ya da ön ekini yer değiştiriseniz, bu durumda APC ön belleğini
    boşaltmanız gerekir. Aksi takdirde autoloader hala ilgili namespace içerisindeki
    tüm sınıfların eski yerlerine bakacaktır.

.. index::
   single: Performans; Bootstrap(Ön Yükleme) dosyaları

Bootstrap(Ön Yükleme) dosyalarını kullanmak
-------------------------------------------

Optimal esneklik ve kod yeniden kullanımını sağlamak için Symfony2 
uygulamaları pek çok sınıf  ve 3.parti bileşen ile birlikte bunu sağlar.
Fakat ayrı dosyalar içerisinde bulunan tüm dosyaları her istek geldiğinde
yüklemek ek bir yük getirecektir. Bu yükü azaltmak için Symfony2 Standart
Sürüm çoklu sınıf tanımlarının tek bir dosyada oluşturulmasını sağlayan 
`bootstrap file`_ script ile birlikte gelir. Bu dosyanın içindekiler ile
(pek çok çekirdek sınıfın kopyalarını barındırır) artık Symfony2 bu sınıfları
içeren dosyaları çağırmak zorunda kalmayacaktır. Bu disk IO'sunu epey bir
azatlacaktır.

Eğer Symfony2 Standart Sürüm kullanıyorsanız muhtemelen zaten
bir bootstrap dosyası kullanıyorsunuz. Emin olmak için front controller'inizi
acın (genellikle ``app.php``) ve şu satırın varlığını kontrol edin::

    require_once __DIR__.'/../app/bootstrap.php.cache';

bootstrap Dosyasını kullanırken iki dez avantajınız olduğunu dikkate alın:

* dosya herhangi bir orijinal kaynak değiştiğinde yeniden yaratılması gerekir.
  (örn: Stmfony2'nin ya da vendor kütüphanelerinin dosyaları güncellenirse) 

* hata ayıklama sırasında bootstrap dosyası içerisinde bir break point
  gereklidir. 

Symfony2 Standart Sürüm kullanıyorsanız bootstrap dosyası vendor kütüphanelerini
``php bin/vendors install`` komutu ile güncelledikten sonra otomatik olarak
yeniden yapılacaktır.

Bootstrap Dosyaları ve Byte Code Ön Bellekleri
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Byte code ön belleklemesi kullanılsa bile performans değişiklikler daha 
az dosya da izleneceğinden dolayı bootsrap dosyası kullanıldığında artacaktır.
Elbette eğer bu özellik devre dışı bırakılırsa (örn: APC'de ``apc.stat=0`` yapılırsa)
bootstrap dosyasının kullanılmasınında bir anlamı kalmayacaktır.

.. _`byte code ön bellekleme`: http://en.wikipedia.org/wiki/List_of_PHP_accelerators
.. _`APC`: http://php.net/manual/en/book.apc.php
.. _`autoloader.php`: https://github.com/symfony/symfony-standard/blob/master/app/autoload.php
.. _`bootstrap file`: https://github.com/sensio/SensioDistributionBundle/blob/2.0/Resources/bin/build_bootstrap.php
