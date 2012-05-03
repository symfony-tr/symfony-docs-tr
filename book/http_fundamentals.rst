.. index::
   single: Symfony2 Temelleri

Symfony2 ve HTTP Temelleri
==============================
Tebrikler! Symfony2 hakkında öğrenerek, daha üretken, çok yönlü ve popüler 
(aslında, siz kendiniz en son kısımdasınız) bir web geliştiricisi olma 
yolunda doğru yol almaktasınız. 
Daha hızlı ve sağlam uygulamalar geliştirmek için Symfony2 temellere 
geri dönerek alışılmışın dışında geliştirme araçlarına sahiptir.
Symfony binlerce insanın yıllarca çalışmasının ürünü olan pek çok teknolojinin
en iyi fikirlerini ,yardımcı araçlarını ve konseptlerini bünyesinde barındırır.
Diğer bir kelimeyle "Symfony" öğrenirken web temellerini öğrenecek, en iyi
örneklerle geliştirme yapacak ve Symfony2 ile bağılmı ya da bağımsız olan 
pek çok inanılmaz PHP kütüphanesininde nasıl kullanılacağını öğreneceksiniz.
Şimdi başlayalım.

Web geliştirmede genel konsept olan HTTP konusunu Symfony2'nin felsefesine uygun olarak
açıklamaya başlayalım.
Genel bilgi düzeyiniz ya da tercih ettiğiniz programlama dilinden bağımsız olarak
bu kısmı herkezin **okuması gereklidir** .

HTTP Basittir
--------------
HTTP (geekler için Hypertext Transfer Protocol'ü demek)  iki makinenin birbiri
arasında konuşması için kullanılan metin tabanlı bil dildir. Hepsi bu kadar !.
Örneğin ne zaman en son `xkcd`_ karikatürlerini görmek istediğinizde şu (kabaca)
konuşma geçer:

.. image:: /images/http-xkcd.png
   :align: center

Kullandığınız gerçek dilde bu biraz daha karmaşık iken aslında iş temelde bu kadar basittir.
HTTP bu basit metin tabalı dilin kurallarını tanımlar. Http sizin nasıl web
geliştirme yaptığınızla ilgilenmez.Ama amacı sunucunuzun *her zaman* basit 
metin tabanlı istekleri anlayarak basit metin tabanlı cevaplar geri döndürmesini
sağlamaktır.
Symfony2'de işte bu durum üzerine inşaa edilmiştir. HTTP'yi nasıl kullandığınızın
önemi yoktur. Symfony2 ile HTTP konusunda nasıl uzmanlaşacağınızı öğreneceksiniz.

.. index::
   single: HTTP; İstek-cevap yaklaşımı

Adım 1: İstemci Bir İstek Gönderir
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web'de her konuşma bir *istek (request)* ile başlar. İstek istemci (client)
tarafından (Örn: bir tarayıcı, bir iPhone uygulaması vb...) yaratılan 
HTTP olarak bilinen özel formatta bir metin tabanlı mesajdır. İstemci
sunucuya bir istek (request) gönderir ve cevabını bekler.

Birinci kısımdaki tarayıcı ve xkcd sunucusu arasındaki olaya geri dönelim:

.. image:: /images/http-xkcd-request.png
   :align: center


HTTP konuşmasında bu HTTP isteği aslında şuna benzemektedir:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)


Bu basit mesaj istek yapan istemci hakkındaki bilgilerin neredeyse *tamamını*
kapsamaktadır. HTTP isteğinin ilk satırı çok önemli iki şeyi barındırır: URI 
ve HTTP metodu.

URI (örn: ``/``, ``/contact`` vb..) istemcinin istediği kaynağın benzersiz 
bir şekilde tanımlandığı konumu ifade eder. HTTP metodu (örn: ``GET``)
bu kaynakla ne *yapmak* istediğinizi ifade eder. HTTP metodları isteklerin
bir kaç genel bilinen yolla kaynak üzerinde nasıl davranılacağını gösteren
eylemlerdir :

+----------+---------------------------------------+
| *GET*    | Sunucudan kaynağı alır.               |
+----------+---------------------------------------+
| *POST*   | Sunucuda bir kaynak oluşturur.        |
+----------+---------------------------------------+
| *PUT*    | Sunucudaki kaynağı günceller          |
+----------+---------------------------------------+
| *DELETE* | Sunucudaki kaynağı siler              |
+----------+---------------------------------------+

Bu duruma göre belirlenen bir blog girdisinin silinmesini HTTP isteği ile
şu şekilde olabileceğini düşünebilirsiniz:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    Aslında HTTP tanımlamasında dokuz adet HTTP metodu tanımlanmış olmasına
    karşın pek çoğu artık kullanılmamakta ya da desteklenmemektedir.
    Gerçekte pek çok modern tarayıcı ``PUT`` ve ``DELETE`` metodlarına
    destek vermezler.

İlk satıra ek olarak bir HTTP isteği daima istek başlıklarının içerdiği
diğer tüm satırları içerir.Bu başlıklar istekte bulunan ``Host`` 'un
geniş çaptaki bilgilerin, istemcinin kabul ettiği cevap formatlarını 
(``Accept``) ve istemcinin istek yaptığı uygulama (``User-Agent``)
gibi pek çok bilgi içerir. Diğer başlıklar hakkındaki bilgiler
Wikipedia'nın `HTTP başlık alanları listesi`_ adlı makalesinde bulunmaktadır.

Adım 2: Sunucu bir cevap döndürür
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sunucu bir istek alır almaz istemcinin hangi kaynağı istediği (URI aracılığı ile)
ve hangi istemcinin bu kaynakla ne yapmak istediğini (metod aracılığı ile) bilir.
Örneğin GET isteği durumunda sunucu kaynağı HTTP cevabı olarak hazırlar.  xkcd
web sunucusunu göz önüne aldığımızda :

.. image:: /images/http-xkcd.png
   :align: center

HTTP çevrilen cevap tarayıcıda şu şekilde gözükecektir:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- xkcd karikatürü için HTML kodu -->
    </html>


HTTP cevabı istenen kaynağın bilgisini barındırdığı gibi (bu örnekte HTML
içeriğidir) aynı zamanda cevap hakkında diğer bilgileri de barındırır.
İlk satır özellikle HTTP durum kodunu gösteren önemli bir kısımdır. (
Bu örnekte 200) Bu durum kodu ile dönüş yapılacak istemci arasında iletişim
kurulur. İstek başarılı oldumu ? Bir hata var mı ? Farklı bir durum kodu 
çıktığında bunun hata mı, başarılı bir istek olduğumu ya da istemcinin başka
bir şey istediğini mi (örneğin başka bir sayfaya yönlendirme) olduğu böylece
belirlenmiş olur. Durum kodları hakkındaki tam liste Wikipedia'nın 
`HTTP durum kodları`_  makalesinde bulunabilir.

İstekte olduğu gibi HTTP cevapları (response) HTTP başlıkları (headers)
olarak bilinen ayrıca ek bilgiler içerir. Örneğin önemli bir HTTP cevap
başlığı ``Content-Type`` dır. Aynı kaynağın içeriği HTML, XML, ya da  JSON 
olarak döndürülebilir. ``Content-Type`` başlığına  ``text/html`` gibi
Internet Medya tipleri bilgisi atanarak istemciye bu kaynağın nasıl 
döndürüleceği belirlenir.  Internet medya tipleri hakındaki tüm listeye
Wikipedia'nın `Genel medya tipleri listesi`_ makalesinden erişilebilir.

İçlerinde oldukça güçlü olan diğer başlık tanımlamalarıda mevcuttur. 
Örneğin bazı başlıklar güçlü bir önbellekleme (caching) sistemi için 
kullanılır.


İstekler, Cevaplar ve Web Geliştirme
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bu istek-cevap iletişimi web üzerindeki tüm iletişimin temelini sağlayan
bir süreçtir. Önemli olan şey, bu iletişimin çok basit
bir şekile ve oldukça güçlü olarak sağladığıdır.

Burada anlaşılması gereken en önemli şey sizin uygulama dilinden 
ve uygulama tipinizden (web, mobile, JSON API) ya da geliştirme mantalitenizden
bağımsız olarak bir uygulamanın **daima** her isteği anladığı ve uygun
bir cevabı geri döndürdüği durumu olmalıdır.

Symfony işte bu gerçek üzerine mimarilendirilmiştir.

.. tip::

    HTTP tanımlaması hakkında daha fazlasını öğrenmek için orijinal 
    `HTTP 1.1 RFC`_ ya da `HTTP Bis`_, belgelerini okumanız gereklidir.
    İstek ve cevap başlıklarını kontrol edebilmek için Firefox'un `Live HTTP Headers`_ 
    adlı eklentisi mükemmel bir yardımcı araçtır.

.. index::
   single: Symfony2 Temelleri; İstekler ve cevaplar

PHP'de İstekler ve cevaplar
-----------------------------
Peki PHP kullanarak "istekler" 'i nasıl yapacak ve "cevaplar" 'ı nasıl 
yaratacağız?. Gerçekte PHP bu süreci kısa bir şekilde ifade eder:

.. code-block:: php

    <?php
    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'The URI requested is: '.$uri;
    echo 'The value of the "foo" parameter is: '.$foo;

Bu acayip gözüken küçük uygulama aslında bir HTTP isteğini alıyor ve
buna göre bir HTTP cevabı yaratıyor.
HTTP 'nin ham mesajlarını işlemek yerine PHP ``$_SERVER`` ve ``$_GET``
adı verilen süper global değişkenleri ile isteğin tüm bilgilerini hazırlıyor.
Benzer olarak geri dönen HTTP formatlı metin mesajı ile ``header()`` 
fonksiyonunu kullanarak cevap başlıkları (reponse header) yaratarak basitçe
güncel içeriği gelen cevap mesajına göre ekrana basabiliyorsunuz. Aşağıda 
PHP istemciye gidecek gerçek bir HTTP cevabı yaratacak:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    The URI requested is: /testing?foo=symfony
    The value of the "foo" parameter is: symfony

Symfony'de İstekler ve cevaplar
---------------------------------
Symfony, PHP'nin HTTP istekleri ve cevapları'nın arasındaki iletişimi
sağlamak için kullandığı yaklaşımı alternatif bir yok kullanarak kolaylıkla
gerçekleştirir. :class:`Symfony\\Component\\HttpFoundation\\Request` sınıfı
HTTP istek mesajlarının ifade edildiği basit bir sınıftır.
Bu sınıf ile isteğe (request) ait bilgileri parmaklarınızın ucunda olacaktır::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // the URI being requested (e.g. /about) minus any query parameters
    $request->getPathInfo();

    // retrieve GET and POST variables respectively
    $request->query->get('foo');
    $request->request->get('bar', 'default value if bar does not exist');

    // retrieve SERVER variables
    $request->server->get('HTTP_HOST');

    // retrieves an instance of UploadedFile identified by foo
    $request->files->get('foo');

    // retrieve a COOKIE value
    $request->cookies->get('PHPSESSID');

    // retrieve an HTTP request header, with normalized, lowercase keys
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // an array of languages the client accepts

``Request`` sınıfı arka tarafta çok iş yapar diye endişelenmeyin.
Örneğin ``isSecure()`` metodu kullanıcının güvenli bağlantı yapıp 
yapmadığını (Örn: ``https``) anlamak için sadece PHP'deki üç farklı 
değere bakar.

.. sidebar:: ParameterBags ve Request nitelikleri

    Yukarıda ``$_GET`` ve ``$_POST`` değişkenlerinin sırasıyla ``query`` 
    ve ``request`` özellikleri ile erişebildiğini gördünüz. Bu nesnelerin
    her birisi :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
    nesnesinin aşağıdaki gibi kullanılan metodlarıdır
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` ve diğerleri.
    Aslında önceki örnekte kullanılan her bir özellikte ParameterBag'ın bir 
    özelliğidir.
    
    .. _book-fundamentals-attributes:
    
    Request sınıfı ayrıca ``attributes`` adındaki özelliği ile uygulamanın
    kendi içerisinde kullanılmak üzere bazı ekstra bilgileride tutar.
    Symfony2 framework'u için ``attributes`` içeriği,eşleşen yönlendirme
    için ``_controller`` bilgisi, ``id`` (eğer yönlendirme de ``{id}`` parametresi 
    kullandıysanız ) ve eşleşen yönlendirme ismi (``_route``) dir.
    ``attributes`` özelliği'nin tuttuğu bilgileri istediğiniz yerde kullanabilir
    ve isteğe göre içerik özel olarak tutabilirsiniz.
      

Symfony ayrıca ``Response`` adında HTTP response mesajları için bir sınıf barındırır. 
Bu uygulamanızda istemciye dönecek olan mesajları nesne tabanlı bir arabirimle 
inşa etmenize olanak sağlar::

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(200);
    $response->headers->set('Content-Type', 'text/html');

    // prints the HTTP headers followed by the content
    $response->send();

Eğer Symfony başka bir şey teklif etseydi siz zaten bu request bilgisine 
ulaşıp cevap yaratmak için nesne yönelimli araç kullanacaktınız. Symfony
içerisinde gelen pek çok güçlü özellikten de öğrendiğiniz üzere ana amaç,
uygulamanızın *gelen isteği yorumlamak ve uygulama mantığınız içerisindeki en
uygun cevabı yaratmaktır.*

.. tip::

    ``Request`` ve ``Response`` sınıfları kendi başına çalışabilen ve Symfony'de
    ``HttpFoundation`` olarak adlandırılan bileşendedirler. Bu bileşen. 
    Symfony'den bağımsız olarak oturumlar ve dosya yüklemeleri içinde başka uygu
    lamalarda kullanılabilir.
    

İstekten Cevaba Bir Seyahat
------------------------------
HTTP'ninde olduğu gibi ``Request`` ve ``Response`` nesneleri oldukça basittir.
Bir uygulamanın geliştirilmesinin zorluğu nelerin gelip gittiğinin yazılmasıdır.
Diğer bir ifade ile gerçek çalışmalar istek bilgilerini yorumlar ve ilgili bilgi
yüklü cevapları oluşturur.

Uygulamanız muhtemelen e-posta göndermek form verilerini işlemek, veri tabanına
bilgi saklamak, HTML sayfalarını oluşturmak ve içeriğinizin güvenliğini sağlamak
gibi pek çok iş yapıyordur. Bunları ortak bir yapıda nasıl organize edip bakımını
sağlayabilirsiniz ?

Symfony ortadaki bu sorunları çözerek size bir şey bırakmaz.

Front Controller
~~~~~~~~~~~~~~~~~~~~

Geleneksel olarak web uygulamalarında her sayfa bir dosya ile ifade edilir :

.. code-block:: text

    index.php
    contact.php
    blog.php

Bu yaklaşımda esnek olmayan URL'ler (``blog.php`` den ``news.php`` ye değişiklik yaparken 
tüm linklerinizi bozmadan değişklik nasıl yapabilirsiniz ), her dosyanın içerisinde 
mutlaka manuel olarak güvenlik, veritabanı bağlantıları ve sitenin
görünümü gibi bazı çekirdek dosyaların sürekli çağrılmak zorunda kalınması
gibi bazı sorunlar ortaya çıkar. 

En iyi çözüm tek bir php dosyasından oluşan ve gelen her isteği işleyen bir
:term:`front controller` kullanmaktır. Örneğin:

+------------------------+------------------------------+
| ``/index.php``         | ``index.php`` 'yi çalıştırır |
+------------------------+------------------------------+
| ``/index.php/contact`` | ``index.php`` 'yi çalıştırır |
+------------------------+------------------------------+
| ``/index.php/blog``    | ``index.php`` 'yi çalıştırır |
+------------------------+------------------------------+

.. tip::

    Apache web sunucusunun ``mod_rewrite`` modülünü kullandığınızda
    (ya da diğer web sunucularındaki bu işe yarayan bir şeyi),
    URL'ler kolaylıkla temizlenip sadece ``/``, ``/contact`` ve
    ``/blog`` şekline gelebilirler.

Şimdi her istek aynı şekilde işlenir. Birbirinden bağımsız URL'lerin 
farklı PHP dosyaları tarafından çalıştırılması yerine front controller 
*daima* tek olarak çalışır ve içeride uygulamanın farklı parçalarındaki farklı
URL'lere yönlendirme yapar. Bu orijinal bir yaklaşımla bu iki sorun çözülür.
Modern tüm web uygulamaları -WordPress gibi- bu sorunu tamamen bu şekilde
çözerler.

Düzenli Kalmak
~~~~~~~~~~~~~~
Fakat front controllerinizin içerisinde hangi sayfanın aynı şekilde 
ekrana basılmasını nasıl sağlayacaksınız ?. Bu yollardan birtanesi
gelen URI'yi kontrol etmek ve gelen değere göre kodunuzun farklı kısımlarını
çalıştırmak olabilir. Bu 

But inside your front controller, how do you know which page should
be rendered and how can you render each in a sane way? One way or another, you'll need to
check the incoming URI and execute different parts of your code depending
on that value. Bunu çok çirkin olarak şu şekilde yapabilirsiniz:

.. code-block:: php

    // index.php

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // the URI path being requested

    if (in_array($path, array('', '/')) {
        $response = new Response('Welcome to the homepage.');
    } elseif ($path == '/contact') {
        $response = new Response('Contact us');
    } else {
        $response = new Response('Page not found.', 404);
    }
    $response->send();


Bu problemi çözmek zor olabilir. Ama neyseki Symfony tamamen bunu çözmek 
için tasarlanmıştır.

Symfony Uygulama Akışı
~~~~~~~~~~~~~~~~~~~~~~
Gelen her isteği Symfony ye bıraktığınızda hayat daha da kolaylaşır. Symfony
her istek için aynı şablonu kullanır:

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 istek akışı

   Gelen istekler yönlendirme tarafından işlenir ve controllerdaki
   ``Response`` nesnesini çeviren fonksiyona gönderilir.

Sitenizin yönlendirme konfigürasyon dosyasında tanımlı olan her "sayfa"
farklı URL adresleri için farklı PHP fonksiyonlarına yönlendirilir. Her
PHP fonksiyonunun işi istekten -Symfony de mevcut olan diğer toolarda dahil
olmak üzere- gelen bilgiyi kullanıp geriye bir ``Response``
nesnesi çeviren bir :term:`controller` 'ı çalıştırmaktır.
Diğer bir ifade ile controller, *sizin* bir istek yaratıp geriye bir cevap
döndüren kodunuzdur.

Ne kadar kolay!.Tekrar Edelim:


* Her istek bir front controller dosyasını çalıştırır;

* Yönlendirme sistemi,istekten gelen bilgilerle konfigürasyonda belirlediğiniz
  PHP fonksiyonlarından hangisinin çalışacağını belirler;

* Kodunuz içerisinde belirlediğiniz, uygun ``Response`` nesnesini çeviren
  PHP fonksiyonu çalıştırılır.

Uygulamada Bir Symfony İsteği
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Çok fazla detaya girmeden uygulamada sürecin nasıl işlediğine bakalım.
Varsayalım Symfony uygulamanıza ``/contact`` sayfası eklemek istiyorsunuz.
Öncelikle yönlendirme konfigürasyon dosyanıza  ``/contact``  girdisini 
eklemelisiniz:

.. code-block:: yaml

    contact:
        pattern:  /contact
        defaults: { _controller: AcmeDemoBundle:Main:contact }

.. note::

   Bu örnek yönlendirme konfigürasyonu için :doc:`YAML</components/yaml>`
   kullannır. Yönlendirme konfigürasyonu ayrıca PHP ya da XML tiplerinde de 
   yazılabilir.

Birisi ne zaman ``/contact`` sayfasını ziyaret ederse bu yönlendirme eşleşecek
ve belirlenmiş olan controller çalıştırılacaktır.:doc:`Yönlendirme Kısmında</book/routing>`,
öğrendiğiniz gibi ``AcmeDemoBundle:Main:contact`` string değeri ``MainController`` adı ile
belirlenmiş olan sınıfın içerisindeki ``contactAction`` adlı PHP metodunun kısa yazımıdır:

.. code-block:: php

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contact us!</h1>');
        }
    }

Bu çok basit örnekte controller basitçe HTML olarak "<h1>Contact us!</h1>" 
içeren bir ``Response`` nesnesi yaratmaktadır. :doc:`Controller kısmında</book/controller>`,
bir controller'in "sunum" kodunuzu içeren (örn. HTML'den başka birşey olmayan) ayrı bir dosya içerisinde 
tutulan şablonu ekrana nasıl basabileceğini öğreneceksiniz.
Bu özellik controller'ı veri tabanı ile etkileşime geçmek, gelen veriyi işlemek 
ya da e-posta mesajları göndermek gibi daha ciddi işleri yapmak konusunda 
rahatlatır.

Symfony2: Yardımcı araçlarınızı Değil Uygulamanızı Geliştirin.
---------------------------------------------------------------
Bildiğiniz gibi her uygulamanın ana amacı gelen her istek için uygun
bir cevap yaratmaktır. Uygulama büyüdüğünde kodunuzu düzenlemek ve
derli toplu kalmasını sağlamak zorlaşır. Her zaman veritabanına bir
şeyler kayıt etmek, şablonları ekrana basmak ya da yeniden kullanmak,
form verileri almak ve işlemek,e-postalar göndermek, güvenlik amacıyla
kullanıcı girdileri kontrol etmek ve düzenlemek gibi aynı karmaşık görevler
tekrar tekrar karşınıza gelir.
İyi haber bu sorunların hiç birisi benzersiz çözümlere sahip değildir. 
Symfony uygulamanızı geliştirmek için bir framework sağlar, bunlar için
kullanacağınız yardımcı araçlar geliştirmek için değil... Symfony2 size
Symfony2 'nin tamamını kullanmanız konusunda bir zorlama yapmaz. Tamamını
kullanmak ya da bir parçasını kullanmak konusunda özgürsünüz.


.. index::
   single: Symfony2 Temelleri

Kendi Başına Çalışan Araçlar: Symfony2 *Bileşenleri* (Components)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Peki Symfony2 *nedir?* Öncelikle Symfony2 yirminin üzerinde birbirinden 
bağımsız herhangi bir PHP uygulamasında kullanılabilecek olan kütüphaneden
oluşan bir kolleksiyondur.
Bu kütüphaneler her durumda ,projenizi nasıl geliştirdiğinize aldırmayan
işinize yarayabilecek kullanışlı şeyler içeren, *Symfony2 Bileşenleri* 
olarak adlandırılır. 

Bunlardan bir kaç tanesinin adı:

* `HttpFoundation`_ - ``Request`` ve ``Response`` sınıfları ile
  oturum yönetimi ve dosya yükleme işlemlerini de içeren bileşen;

* `Routing`_ - Spesifik URI'leri yöneten (Örn. ``/contact``) , gelen isteğin nasıl
  işleneceğini belirleyen (Örn: ``contactAction()`` metodunu çalıştıran) güçlü ve hızlı
  yönlendirme sistemi;

* `Form`_ - Form'ları yaratan ve form verilerini işleyen tam donanımlı 
   esnek bir form framework'u;

* `Validator`_ kullanıcı tarafından girilen verilerin yarattığınız veri doğrulama
  kuralları ile uyuşup uyuşmadığını kontrol eden bir sistem;

* `ClassLoader`_ PHP sınıflarını içeren her dosyayı diğer dosyalar içerisinde manuel olarak yükleme
  zahmetinden kurtaran bir otomatik yükleme kütüphanesi;

* `Templating`_ Şablonları ekrana basmak, şablon arabirimini kontrol etmek ve
  diğer temel şablon işlemlerini gerçekleştirmek için kullanılan bir toolkit;

* `Security`_ - Uygulamanızda güvenlik ile ilgili gerek duyduğunuz tüm işlemleri
  yapabileceğiniz güçlü bir kütüphane;

* `Translation`_ Uygulamanızdaki metinleri diğer dillere çevirebileceğiniz bir kütüphane.

Bu bileşenlerden her birisi Symfony2 frameworkunu kulllanıp kullanmadığınıza
bakılmaksızın *herhangi bir* PHP projesinde parça parça kullanılabilir.
Her parça gerekli olduğu zaman yeniden düzenlenebilir.

Tam Çözüm: Symfony2 *Framework*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Peki Symfony2 *Framework* 'ü *nedir* ? *Symfony2 Framework* 'ü 
aşağıdaki iki ayrı görevi gerçekleştiren bir PHP kütüphanesidir :

#. Seçilmiş bileşenleri (Symfony2 Bileşenleri) 
   ve üçüncü parti kütüphaneleri (Örn. e-posta göndermek için``Swiftmailer``) 
   sağlamak;

#. Tutarlı bir konfigürasyon yapısı ve bu yapıyla birlikte tüm parçaları 
   biribirine "tutkal" gibi yapıştıran kütüphaneler sağlamak. 

Framework'un amacı pek çok birbirinden bağımsız yardımcı aracı geiştirici
için en uygun ve tutarlı bir şekilde entegre etmektir. Bu bir Symfony2 Bundle'ı
(plugin) içerisinde kolaylıkla konfigüre edilebilir.

Symfony2, uygulamanızı zora sokmadan hızlı bir şekilde web uygulamaları geliştirmenize
olanak sağlayan pek çok güçlü yardımcı araç sağlar. Normal kullanıcılar hızlı bir şekilde
Symfony2 dağıtımı kullanarak mantık çerçevesi belirlenmiş olan projeleri için uygulama
geliştirebilirler. İleri düzey kullanıcılar için ise limit gökyüzüdür.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/3829/
.. _`HTTP durum kodları`: http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
.. _`HTTP başlık alanları listesi`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`Genel medya tipleri listesi`: http://en.wikipedia.org/wiki/Internet_media_type#List_of_common_media_types
.. _`HttpFoundation`: https://github.com/symfony/HttpFoundation
.. _`Routing`: https://github.com/symfony/Routing
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`ClassLoader`: https://github.com/symfony/ClassLoader
.. _`Templating`: https://github.com/symfony/Templating
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
