:orphan:

Sözlük
======

.. glossary::
   :sorted:

   Dağıtım
        Bir *Dağıtım* Symfony2 bileşenleri, seçilmiş bir kaç bundle, iyi
        düzenlenmiş bir dizin yapısı, varsayılan konfigrasyon ve isteğe
        bağlı konfigürasyonların paketlenmiş halidir.

   Proje
        Bir *Proje* Uygulamanın yazıldığı klasör, bir dizi bundle, vendor
        kütüphaneleri,bir autoloader ve bir web front controller scriptinden
        oluşur.

   Uygulama
        Bir *Uygulama*, verilen bir dizi Bundle'ın *konfigürasyon* 'larını
        içeren bir klasördür.

   Bundle
        Bir *Bundle* tek bir özelliği *yapılandıran* 
        (bir blog, bir forum vb..) dosyaları içeren bir
        (PHP dosyaları, stil şablonları, Javascriptler, resimler...) 
        klasördür. Symfony2'de (*daima*) herşey bir bundle içerisindedir.
        (bkz :ref:`page-creation-bundles`)

   Front Controller
        Bir *Front Controler* projenizin web dizini içerisinde bulunan bir
        PHP script'idir. Tipik olarak *tüm* istekler işi Symfony uygulamasını
        çalıştırmakla aynı front controller tarafından işletilir ve çalıştırılır.

   Controller
        Bir *controller* bir sayfayı temsil eden ``Response`` nesnesinin
        içeriğini yaratmak için gerekli olan tüm algoritmayı içereen
        bir PHP fonksiyonudur. Tipik olarak bir route bir controller ile
        istekten gelen bilgiyi alıp işlemek için, aksiyonları gerçekleştirmek
        için ve bir ``Response`` nesnesini geri döndürmek için eşleşir. 

   Servis
        Bir *Servis* özel bir işlemi gerçekleştiren jenerik bir PHP sınıfını
        tanımlar. Bir servis genellikle veritabanı bağlantısı nesnesi ya da 
        e-posta mesajlarını almak gibi genel işler için kullanılır. Symfony2'de
        servisler sıklıkla servis kutusu tarafından konfigüre edilirler ve
        çağırılırlar. Bir uygulama pek çok küçük serviste oluşması bunun
        `servis tabanlı mimari`_ ye sahip olduğunu söyler.
        
   Servis Kutusu
        Bir *Servis Kutusu* aynı zamanda bir uygulamanın içerisindeki
        servislerin başlatılmasından sorumlu özel bir nesne olan 
        *Dependency Injection Container* olarak da bilinir. Servisleri
        direkt olarak yaratmak yerine geliştirici servis kutusu içerisinde
        (konfigürasyonlar aracılığu ile) servislerin nasıl yaratacağını
        *geliştirir*. Servis kutusu bağlımlı servisleri gerektiğinde 
        uygulamaya sokmaya dikkat eder (laizly loading). Bu konudaki bilgi
        için :doc:`/book/service_container` kısmına bakınız.

   HTTP Şartnamesi
        *HTTP Şartnamesi* Hyper Text Transfer Protocol'unu 
        -klasik sunucu istemci iletişimini bir dizi kural ile- tanımlayan 
        bir belgedir. Tanımlamlar istek ve cevapların mümkün HTTP başlıkları
        ile nasıl oluşturulacağını belirtir. Daha fazla bilgi için 
        `Http Wikipedia`_ makalesini ya da `HTTP 1.1 RFC`_ belgesini okuyun.

   Ortam
        Ortam özel bir dizi konfigürasyonu tanımlayan basit bir karakter
        dizisidir (Örn: ``prod`` ya da ``dev``) Aynı uygulama aynı makinede
        uygulamanın farklı ortamlarında farklı konfigürasyonları kullanarak
        çalışabilir. Bu tek uygulamanın geliştirilme ortamında ``dev`` hata
        ayıklama için kullanıması ve hız için optimize edilmiş ``prod`` ortamında
        çalışmasına olanak verir. 

   Vendor
        Bir *vendor* Symfony2 içerisindeki PHP kütüphanlerini ve bundle'ları
        yaratan destekçileri ifade eder. Ticari hayatta bu kelime farklı
        bir anlam ifade etsede Symfony2'de sıklıkla (hatta genellikle)
        vendor'lar özgür yazılımlar içerir. Symfony2 projesine eklediğiniz
        her kütüphane ``vendor`` dizininde olmalıdır. 
        :ref:`Mimari: Vendorları Kullanmak <using-vendors>` kısmına bakın.

   Acme
        *Acme* Symfony demo ve belgelerinde kullanılan uydurma bir şirkettir.
        Bu normalde name space'ler içerisinde kullanacağınız organizasyonunuzu
        ifade edecek olan kelime yerine geçer (Örn: ``Acme\BlogBundle``).

   Aksiyon
        Bir *aksiyon* örneğin verilen route ile eşlemesi durumunda çalıştırılacak
        bir php fonksiyonu ya da metodudur. Aksiyon termi tüm bir PHP sınıfı 
        ve metodları kapsaması yüzünden genellikle *controller* ile eş anlamlıdır.
		daha fazla bilgi için :doc:`Controller Kısmı </book/controller>` 'na bakınız.
   
   Asset
        Bir *asset* web uygulamasının çalıştırılmayan CSS,Javascript,
        resimler ve video gibi öğeleri içeren statik bileşnlerini ifade eder.
        Asset'ler projenizin ``web`` klasöründe olabileceği gibi :term:`Bundle` 'ınızdan
        ``assets:install`` konsol komutu ile bu klasörede kopyalanabilir.

   Kernel
        *Kernel* Symfony'nin çekirdeğidir. Kernel nesnesi bundle'lar ve
        kütüphanelerin kendisine kayıtladığı tüm HTTP isteklerini işler.
        :ref:`Mimari: Uygulama Klasörü <the-app-dir>` ve :doc:`/book/internals` 
        kısmına bakın.

   Firewall
        Symfony2'de *Firewall* network ile ilgili birşey yapmaz. Bunun yerine
        tüm uygulamanın ya da uygulamanın bir kısmında uygulanacak olan
        yetkilendirme mekanizmasını(kullanıcıların belirlenen kimlik tanım
        lamalarına göre) tanımlar. :doc:`/book/security` kısmına bakınız.

   YAML 
        *YAML* "YAML Ain't a Markup Language" kelimlerinin kısaltılmış halidir.
        Bu, oldukça hafif, Symfony2 konfigürasyon dosyalarında oldukça basit
        anlaşılabilir bir serileştirme sağlar. doc:`/components/yaml` kısmına bakın.


.. _`servis tabanlı mimari`: http://wikipedia.org/wiki/Service-oriented_architecture
.. _`HTTP Wikipedia`: http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
