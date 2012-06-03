.. index::
   single: Testler

Testler
=======

Yeni bir satır kod yazdığınızda potanstiyel olarak yeni hatalar (bug) eklemiş
olursunuz. Daha iyi bir güvenilir uygulamalar geliştirmek için 
kodunuzu fonksiyonel ve unit testleri ile test etmelisiniz.

PHPUnit Test Framework'u
-----------------------------

Symfony2 bağımsız bir kütüphane entegrasyonu ile - PHPUnit olarak adlandırılan -
size zengin bir test framework'u sunar. Bu kısım kendsinin mükemmel bir `belge`_ 'si
olduğundan PHPUnit'in kendisini incelemeyecektir.

.. note::

    Symfony2 PHPUnit 3.5.11 ya da daha üst sürümü ile çalışır.

Her test - unit testi  ya da fonksiyonel bir test - bundle'nızın `Tests/'
klasöründe bulunması gereken bir PHP sınıfıdır. Eğer bu kurala uyarsanız,
uygulamanızın tüm testlerini aşağıdaki komut yardımı ile yapabilirsiniz:

.. code-block:: bash


    # konfigürasyon dizininizi komut satırında belirle
    $ phpunit -c app/

``-c`` seçeneği PHPUnit'e konfigürasyon dosyası için ``app/`` klasörüne bakmasını 
söyler. Eğer PHPUnit seçenekleri hakkında meraklı iseniz ``app/phpunit.xml.dist`` 
dosyasını inceleyin.

.. tip::

	Kod kapsamı ``--coverage-html`` seçeneği ile yaratılabilir.

.. index::
   single: Testler; Unit Testleri

Unit Testleri
-------------

Bir unit testi genellikle belirli bir PHP sınıfına karşı kullanılır. Eğer
tüm uygulamanıza test uygulamak istiyorsanız `Fonksiyonel Testler`_ kısmına bakın.

Symfony2 unit test yazmak standart PHPUnit unit testi yazmaktak farklı değildir.
Varsayalım bundle'ınız içerisindeki ``Utility/`` dizininde ``Calculator``
adında *oldukça* basit bir sınıfınız var::

    // src/Acme/DemoBundle/Utility/Calculator.php
    namespace Acme\DemoBundle\Utility;
    
    class Calculator
    {
        public function add($a, $b)
        {
            return $a + $b;
        }
    }

Bunu test etmek için bundle'ınız içerisinde ``Tests/Utility`` klasörü altında 
``CalculatorTest`` adında bir dosya yaratın::

    // src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php
    namespace Acme\DemoBundle\Tests\Utility;

    use Acme\DemoBundle\Utility\Calculator;

    class CalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testAdd()
        {
            $calc = new Calculator();
            $result = $calc->add(30, 12);

            // iddia ediyoruz calculator rakkamları doğru bir şekilde ekliyor!
            $this->assertEquals(42, $result);
        }
    }

.. note::

    Kural olarak ``Tests/`` alt dizini bundle'ınızın klasörünü kopyalamalıdır.
    Bu yüzden eğer bundle'ınız içerisindeki ``Utility/`` klasörü altında 
    bulunan bir sınıfı test etmek için bu testi ``Tests/Utility/`` klasörüne
    koymalısınız.

Aynı gerçek uygulamanızdaki gibi autoloading ``bootstrap.php.cache`` dosyası
tarafından otomatik olarak aktifleştirilir (eğer ``phpunit.xml.dist``
içerisinde konfigüre edildi ise).

Veriken dosya ya da klasör için testleri çalıştırmak oldukça
kolaydır:

.. code-block:: bash

    # Utility klasöründeki tüm testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/

    # Calculator sınıfı için testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php

    # Bundle'ın tamamında testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/

.. index::
   single: Testler; Fonksiyonel Testler

Fonksiyonel Testler
-------------------

Fonksiyonel testler uygulamanızın farklı katmanlarının entegrasyonunu 
test eder(routing'den views katmanına kadar). Bunların PHPUnit'in yaptığı
unit testlerden hiç bir farkı yoktur ancak oldukça özel bir akışları vardır.

* Bir istek yap (request);
* Cevabı (response) test et;
* Bir linke tıkla ya da bir form verisi gönder;
* Cevabı (response) test et;
* Başa al ve tekrarla;

İlk Fonksiyonel Tesiniz
~~~~~~~~~~~~~~~~~~~~~~~

Fonksiyonel testler genel olarak bundle'ınızın ``Tests/Controller`` 
dizininde bulunan basit PHP dosyalarıdır. Eğer ``DemoController`` class'ınızı
işleten sayfanızı test etmek istiyorsanız yeni bir ``DemoControllerTest.php``
dosyası yaratıp bunu özel ``WebTestCase`` sınıfından genişleterek başlayın.

Örneğin Symfony2 Standart sürüm aşağıdaki gibi ``DemoController`` 
için basit bir fonksiyonel test ile (`DemoControllerTest`_) birlikte gelir::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertGreaterThan(0, $crawler->filter('html:contains("Hello Fabien")')->count());
        }
    }

.. tip::

    Fonksiyonel testleri çalıştırmak için uygulamanızın çekirdeğinde ``WebTestCase``
    sınıfı başlatılma esnasında aktif durumda olmalıdır. Pek çok durumda bu otomatik
    olarak olur. Ancak eğer çekirdeğiniz standart olmayan bir klasörde ise
    ``phpunit.xml.dist`` dosyasındaki ``KERNEL_DIR`` çevre değişkenini kernelinizin
    bulunduğu yere göre düzenlemeniz gerekir::

        <phpunit>
            <!-- ... -->
            <php>
                <server name="KERNEL_DIR" value="/path/to/your/app/" />
            </php>
            <!-- ... -->
        </phpunit>

``createClient()`` metodu bir tarayıcıdan sitenizden bilgi alıyormuş 
gibi davranır::

    $crawler = $client->request('GET', '/demo/hello/Fabien');

``request()`` metodu (bkz :ref:`request metodu hakkında daha fazlası için<book-testing-request-method-sidebar>`)
Response esnasında linklere tıklamak ve form verisi gönderme durumunda nesneleri seçmekte
kullanılan bir  :class:`Symfony\\Component\\DomCrawler\\Crawler` nesnesini döndürür.

.. tip::

    Crawler sadece response XML ya da HTML dokümanı ise çalışır.
    İşlenmemiş response içeriği almak için ``$client->getResponse()->getContent()``
    metodunu çağırın.

Crawler ile seçilen bir XPath ya da CSS seçiçisi kullanan bir linke tıklayın,
sonra istemiciyi ona tıklamak için kullanın. Örneğin aşağıdaki kod, içinde
``Greet`` geçen tüm linkleri bulur sonra ikincisini seçer ve ona tıklar::

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Form verisi göndermekte aynıdır. Bir form butonu seçilir, opsiyonel olarak
bazı form verileri değiştirilir ve ilgili form verisi gönderilir (submit)::

    $form = $crawler->selectButton('submit')->form();

    // Bazı değerleri ata
    $form['name'] = 'Lucas';
    $form['form_name[subject]'] = 'Hey there!';

    // formu gönder
    $crawler = $client->submit($form);

.. tip::

    Form ayrıca dosya yüklemelerini işleyen ve bunları dolduran çeşitli tiplerdeki
    form alanlarına sahiptir (Örn: ``select()`` ve ``tick()``). Detaylı bilgi
    için aşağıdaki `Formlar`_ kısmına bakın.

Şimdi uygulamanızı baştan sonra gezebilirsiniz, bildirimleri (assertion) 
kullanarak gerçekten istediğiniz şeyi verdiğini test edebilirsiniz. DOM 
üzerinde bildirimleri (assertions) yapmak için Crawler kullanın::

    // response'un verilen CSS seçicisiyle eşleştiğini bildir.
    $this->assertGreaterThan(0, $crawler->filter('h1')->count());

Ya da testi, Response'u eğer sadece içeriği bazı metinler kapsayacak 
şekilde de test etmek ya da eğer Response XML/HTML değilse de yapabilirsiniz::

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. _book-testing-request-method-sidebar:

.. sidebar:: ``request()`` metodu hakkında daha fazlası:

	``request()`` metodunun tam açılımı::
	
        request(
            $method,
            $uri, 
            array $parameters = array(), 
            array $files = array(), 
            array $server = array(), 
            $content = null, 
            $changeHistory = true
        )

    ``server`` array'i kolaylıkla PHP'nin `$_SERVER` değişkenindeki değerleri
    içerir. Örneğin `Content-Type` ve `Referer` HTTP başlıklarına değer atamak
    için::

        $client->request(
            'GET',
            '/demo/hello/Fabien',
            array(),
            array(),
            array(
                'CONTENT_TYPE' => 'application/json',
                'HTTP_REFERER' => '/foo/bar',
            )
        );

.. index::
   single: Testler; Bildirimler(Assertions) 

.. sidebar:: Faydalı Bildirimler

    Hızlıca başlamak için burada en sık kullanılan ve faydalı bildirimlerin
    listesi verilmektedir::

        // bir den fazla "subtitle" css sınıfı ile h2 tagı olduğunu bildir.
        $this->assertGreaterThan(0, $crawler->filter('h2.subtitle')->count());

        // Sayfada gerçekten 4 adet h2 tagı olduğunu bildir.
        $this->assertCount(4, $crawler->filter('h2'));

        // "Content-Type" başlığının "application/json" olduğunu bildir.
        $this->assertTrue($client->getResponse()->headers->contains('Content-Type', 'application/json'));

        // Cevap (response) içeriğinin bir düzenli ifade ile eşleştiğini bildir.
        $this->assertRegExp('/foo/', $client->getResponse()->getContent());

        // Response durum kodunun 2xx olduğunu bildir
        $this->assertTrue($client->getResponse()->isSuccessful());
        // Response durum kodunun 404 olduğunu bildir
        $this->assertTrue($client->getResponse()->isNotFound());
        // Özel 200 durum kodunu bildir.
        $this->assertEquals(200, $client->getResponse()->getStatusCode());

        // response'un /demo/contact adresine yönlendirileceğini bildir.
        $this->assertTrue($client->getResponse()->isRedirect('/demo/contact'));
        // ya da sadece cevabın herhangibir URL'ye yönlendirileceğini bildir.
        $this->assertTrue($client->getResponse()->isRedirect());

.. index::
   single: Testler; İstemci (Client)

Test istemcileri (Client) ile çalışmak
--------------------------------------

Test istemcisi tarayıcı gibi bir HTTP istemcisini taklit eder ve Symfony2
uygulamanız içerisinden istekler yapar::

    $crawler = $client->request('GET', '/hello/Fabien');

``request()`` metodu HTTP metodunu alır ve arguman olarak bir URL 'yi bir 
``Crawler`` 'a döndürür.

Use the Crawler to find DOM elements in the Response. These elements can then
Crawler kullanarak Response içerisindeki DOM elementleri bulunur. Bu elementler
daha sonra link tıklaması ya da form göndermesinde kullanılır::

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

``click()`` ve ``submit()`` metodlarının ikiside bir ``Crawler`` nesnesi
çevirir. Bu metodlar formdan HTTP metodu tarama ve dosya yüklemekte
kullanılan güzel API'ler gibi uygulamanızda takip etmeniz gereken pek
çok şeyi sunar.

.. tip::

    ``Link`` ve ``Form`` nesneleri hakkında daha fazla şeyi aşağıdaki
    :ref:`Crawler<book-testing-crawler>` kısmında öğreneceksiniz.
    

``request`` metodu ayrıca form veri göndermelerini direkt olarak simule etmede
ya da daha karmaşık istekleri yaratmada kullanılır::

    // Direkt formu gönder(fakat Crawler kullanmak daha basit!)
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // Dosya yüklemesi
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile(
        '/path/to/photo.jpg',
        'photo.jpg',
        'image/jpeg',
        123
    );
    // ya da
    $photo = array(
        'tmp_name' => '/path/to/photo.jpg',
        'name' => 'photo.jpg',
        'type' => 'image/jpeg',
        'size' => 123,
        'error' => UPLOAD_ERR_OK
    );
    $client->request(
        'POST',
        '/submit',
        array('name' => 'Fabien'),
        array('photo' => $photo)
    );

    // Bir DELETE isteği yap ve HTTP başlıklarına aktar
    $client->request(
        'DELETE',
        '/post/12',
        array(),
        array(),
        array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word')
    );

Son fakat önemli, kendi PHP sürecinde çalışacak olan her isteği,aynı script
içerisinde çeşitli istemcilerle çalışırken yaratacakları farklı etkilerden
kaçınmak için zorlayabilirsiniz:

    $client->insulate();

Taramak (Browsing)
~~~~~~~~~~~~~~~~~~

İstemci gerçek bir tarayıcıda yapılacak pek çok işlemi destekleyebilir::

    $client->back();
    $client->forward();
    $client->reload();

    // Tüm geçmişi ve çerezleri temizler
    $client->restart();

İçsel Nesnelere Erişmek
~~~~~~~~~~~~~~~~~~~~~~~

Eğer uygulamanızı test etmek için istemci kullanıyorsanız istemcinin içsel
nesnelerine ulaşmak isteyebilirsiniz::

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

Ayrıca son istekle ilgili olan nesneleride alabilirsiniz::

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

Eğer istekleriniz izole edilmediyse ayrıca ``Container`` ve ``Kernel`` 'a
da erişebilirsiniz::

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Container'a Erişmek
~~~~~~~~~~~~~~~~~~~~~~~

Fonksiyonel testi sadece Response'un testinde kullanılması şiddetle tavsiye
olunur.Fakat bazı çok özel ve nadir durumlarda, bildirimler(assertion) yazmak
için bazı içsel nesnelere erişmek isteyebilirsiniz. Bu gibi durumlarda
bağımlılık aşısı container'ına erişebilirsiniz (dependency injection container)::


    $container = $client->getContainer();

Dikkat edin!. Bu eğer istemciyi izole ettiyseniz ya da eğer HTTP katmanı
kullandıysanız çalışmaz. uygulamanızda mevcut olan servislerin listesi için
``container:debug`` konsol komutunu kullanın.

.. tip::

    Eğer kontrol etmek istediğiniz bilgi profiler'da ise o zaman onu kullanın.

Profiler Verisine Erişmek
~~~~~~~~~~~~~~~~~~~~~~~~~

Her istekte Symfony profiler'ı bu isteğin içsel olarak işlenmesi ile ilgili
pek çok bilgiyi toplar. Örneğin profiler, verilen sayfa çalıştırma sayısının
ilk yüklenme esnasında belirli sayıdaki veritabanı sorgusundan küçük 
olduğunu kontol etmekte kullanılabilir.


Profiler'dan son isteği almak için şunu yapın::

    $profile = $client->getProfile();

Test esnasında profiler'ı kullanmak hakkındaki özel bilgiler için 
:doc:`/cookbook/testing/profiling` tarif kitabı girdisine bakın.

Yönlendirme (Redirecting)
~~~~~~~~~~~~~~~~~~~~~~~~~

Bir request bir yönlendirme cevabı döndürdüğünde istemci bunu
otomatik olarak takip etmez. Response ve zorla yönlendirmeyi daha sonra
``followRedirect()`` metoduyla test edebilirsiniz::

    $crawler = $client->followRedirect();
    
Eğer istemcinin tüm yönlendirmeleri takip etmesini istiyorsanız onu ``followRedirects()``
metodu ile bunu yapmaya zorlayabilirsiniz::

    $client->followRedirects();

.. index::
   single: Testler; Crawler

.. _book-testing-crawler:

Crawler
-----------

Bir Crawler örneği (instance) her seferinde istemci ile yaptığınız isteği
çevirir. Bu  size HTML dokümanlarını , seçim düğümlerini linkleri ve
formları bulmayı işlerini yakalamaya (travers) olanak sağlar.

Yakalamak(Traversing)
~~~~~~~~~~~~~~~~~~~~~

jQuery gibi Crawler HTML/XML belgesinin DOM'larını yakalayan metodları vardır.
Örneğin aşağıda tüm ``input[type=submit]`` elementi bulunur, sayfadaki en son
olanı seçilir ve ilk üst elementi bulunur.

    $newCrawler = $crawler->filter('input[type=submit]')
        ->last()
        ->parents()
        ->first()
    ;

Başka diğer metodlarda mmevcuttur:

+------------------------+----------------------------------------------------+
| Metod                  | Açıklaması                                         |
+========================+====================================================+
| ``filter('h1.title')`` | Eşleşen CSS seçicisinin düğümleri                  |
+------------------------+----------------------------------------------------+
| ``filterXpath('h1')``  | Eşleşen XPath ifadesinin düğümleri                 |
+------------------------+----------------------------------------------------+
| ``eq(1)``              | belirlilen index'in düğümü                         |
+------------------------+----------------------------------------------------+
| ``first()``            | İlk düğüm                                          |
+------------------------+----------------------------------------------------+
| ``last()``             | Son Düğüm                                          |
+------------------------+----------------------------------------------------+
| ``siblings()``         | Aynı Türden olanlar (siblings)                     |
+------------------------+----------------------------------------------------+
| ``nextAll()``          | Takip eden tüm aynı türden olanlar                 |
+------------------------+----------------------------------------------------+
| ``previousAll()``      | Önceki tüm aynı türden olanlar                     |
+------------------------+----------------------------------------------------+
| ``parents()``          | Üst Düğümleri Döndür                               |
+------------------------+----------------------------------------------------+
| ``children()``         | Alt düğümleri Döndür                               |
+------------------------+----------------------------------------------------+
| ``reduce($lambda)``    | Çağırılabilen'in false döndürmediği düğümler       |
+------------------------+----------------------------------------------------+

Bu metodların her birisi yeni bir ``Crawler`` örneği (instance) döndürmesine
rağmen düğüm sayınızı metod cağrılarını zincirleyerek daraltabilirsiniz::

    $crawler
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    Crawler içerisinde kaç adet düğüm olduğunu saymak
    için ``count()`` fonksiyonunu kullanın: ``count($crawler)``

Bilgileri Ayıklamak
~~~~~~~~~~~~~~~~~~~

Crawler düğümler üzerinden bilgileri ayıklayabilir::

    // İlk düğüm için nitelik değerini döndürür.
    $crawler->attr('class');

    // İlk düğümün düğüm değeri döner
    $crawler->text();

    // tüm düğümlerin her href değerine uyan niteliklerinin değerlerini 
    // bir array olarak crawler'da ayıkla  (_text nod değerini döndürür)
    $info = $crawler->extract(array('_text', 'href'));

    // her düğüm için çalışacak ve sonucu bir array içerisinde döndürecek
    // bir lambda fonksiyonu çalıştırır
    $data = $crawler->each(function ($node, $i)
    {
        return $node->attr('href');
    });

Linkler
~~~~~~~

Linkleri seçmek için ``selectLink()`` kısayolu ile ifade edilen yakalama
metodu kullanılır::

    $crawler->selectLink('Click here');

Bu verilen metine uyan tüm linkleri seçer ya da ``alt`` niteliğinde verilen
metine uyan tıklanabilen tüm resimleri seçer. Diğer filitreleme metodları 
gibi buda başka bir ``Crawler`` nesnes döndürür.

Bir link seçtiğinizde linkler için özel metodlar barındıran bir ``Link``
nesnesine erişebilirsiniz (``getMethod() ve ``getUri()`` metodları gibi).
Linke tıklamak için istamcinin ``click()`` metodunu kullanın ve bunu
``Link`` nesnesine gönderin::

    $link = $crawler->selectLink('Click here')->link();

    $client->click($link);

Formlar
~~~~~~~

Linkler gibi formlarıda ``selectButton()`` metodu ile seçersiniz::

    $buttonCrawlerNode = $crawler->selectButton('submit');

.. note::

    Bizim formları değilde form butonları seçtiğimize dikkat edin. Eğer bir 
    yakalama (traversing) API'si kullanıyorsanız bunların sadece buton'lara
    baktığını aklınızdan çıkartmayın.

``selectButton()`` metodu ``button`` etiketini ve veri gönderen (submit) ``input``
etiketini seçebilir. Bu metod butonları bulmak için butonların farklı
parçalarını kullanır:

* ``value`` niteliği değeri;

* resimler için ``id`` ya da ``alt`` nitelikleri değeri;

* ``button`` etiketleri için ``id`` ya da ``name`` nitelik değerleri.

Butonu temsil eden bir Crawler'a sahip olduğunuzda ``form()`` metodunu
çağırarak ayrıştılırılmış butonu içeren ``Form`` 'un bir örneğine 
ulaşabilirsiniz.


    $form = $buttonCrawlerNode->form();

``form()`` metodu çağırıldığında alan değerlerinin varsayılan değerleri
ile değiştirmek için bir array içerisinde de verilebilir::

    $form = $buttonCrawlerNode->form(array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

Ve eğer form için özel bir HTTP metodunu simüle etmek istiyorsanız bunu
ikinci argüman olarak verin::

    $form = $buttonCrawlerNode->form(array(), 'DELETE');

İstemci ``Form`` örneklerini submit edebilir::

    $client->submit($form);

Bu alan değerleri aynı zamanda ``submit()`` metodunun ikinci argümanı olarak da
gönderilebilir::

    $client->submit($form, array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

Daha karmaşık durumlar için ``Form`` örneğini bir array değeri içerisinde
her alanı bagımsız olarak verebilirsiniz::

    // Alanın değerini değiştir
    $form['name'] = 'Fabien';
    $form['my_form[subject]'] = 'Symfony rocks!';

Ayrıca Alanların değerlerini kendi tiplerine göre değiştirmek için de güzel
bir API vardır::

    // bir option ya da radio seçer
    $form['country']->select('France');

    // checkbox'u işaretler
    $form['like_symfony']->tick();

    // Bir dosya yükler
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::

    ``Form`` nesnesinde gönderilen değerleri ``getValues()`` metodunu çağırarak
    görebilirsiniz. Yüklenen dosyalar ``getFiles()`` ile bir array halinde döner.
    ``getPhpValues() ve ``getPhpFiles()`` metodlarıda gönderilen(submit) değerleri
    gösterebilir ancak bunları PHP formatında gösterir. (anahtarıları köşeli paranterz
    notasyonuna çevirir Örn : ``my_form[subject]`` 'den PHP array'ına)

.. index::
   pair: Testler; Konfigürasyon

Test Etme Konfigürasyonu
------------------------

İstemci fonksiyonel tesleri özel ``test`` çevresinde çalışan bir Kernel yaratarak
kullanır. Symfony  ``test`` çevresinde ``app/config/config_test.yml`` dosyasını
yüklemesine rağmen uygulamanızın ayarlarını test için de özelleştirebilirsiniz.

Örneğin varsayılan olarak, swiftmailer e-postaları ``test`` çevresinde göndermeye
ayarlı değildir. Bunu ``swiftmailer`` konfigürasyon ayarları altında görebilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        # ...

        swiftmailer:
            disable_delivery: true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <!-- ... -->

            <swiftmailer:config disable-delivery="true" />
        </container>

    .. code-block:: php

        // app/config/config_test.php
        // ...

        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery' => true
        ));

Ayrıca tamamen farklı bir çevre kullanabilir ya da varsayılan hata ayıklama 
modunu (``true`) yaparak her seçeneğin ``createClient()`` 'a aktarılmasını
sağlayabilirsiniz:


    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

Eğer uygulamanız bazı HTTP başlıklarına göre davranıyorsa bunları ``createClient()``
'in ikinci argümanı olarak aktarabilirsiniz::

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

Ayrıca her istek için ayrı ayrı HTTP başlıklarını da değiştirebilirsiniz::

    $client->request('GET', '/', array(), array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    Test istemcisi ``test`` çevresinde container içerisinde bir servis olarak
    mevcuttur( ya da :ref:`framework.test<reference-framework-test>` seçeneği
    nerede aktif edildiyse). Bunun anlamı eğer ihtiyaç duyarsanız bunu
    olduğu gibi düzenleyebilirsiniz.

.. index::
   pair: PHPUnit; Konfigürasyon

PHPUnit Konfigürasyonu
~~~~~~~~~~~~~~~~~~~~~~

Her uygulama kendi ``phpunit.xml.dist`` dosyasında saklanan PHPUnit konfigürasyonuna
sahiptir. Bu dosyadaki varsayılan değerleri düzenleyebilir ya da ``phpunit.xml``
dosyasını yerel bilgisayarınızda yaratarak kendinize göre özelleştirebilirsiniz.

.. tip::

    ``phpunit.xml.dist`` dosyasını kod deponuzda saklayın, ``phpunit.xml``
    dosyasını görmezden gelin.

Varsayılan olarak sadece "standart" bundle'lar içerisinde saklanan testler 
``phpunit`` komutu tarafından çalıştırılır(standart testler ``src/*/Bundle/Tests``
ya da ``src/*/Bundle/*Bundle/Tests`` klasörlerinde olmalıdır). Fakat
kolaylıkla daha farklı dizinler ekleyebilirsiniz. Mesela aşağıdaki konfigürasyon
yüklenen 3.parti bundle üzerinden bir test eklemektedir:
	
.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

Kod kapsayan diğer klasörleri içeri aktarmak için ayrıca ``<filter>`` kısmınıda
düzenlemelisiniz:
	
.. code-block:: xml

    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Tarif Kitabından Daha Fazlasını Öğrenin
----------------------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`


.. _`DemoControllerTest`: https://github.com/symfony/symfony-standard/blob/master/src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
.. _`$_SERVER`: http://php.net/manual/en/reserved.variables.server.php
.. _`belge`: http://www.phpunit.de/manual/3.5/en/
