.. index::
   single: Sayfa Yartımı

Symfony2'de Sayfaları Yaratmak
===============================

Symfony2'de sayfa yaratmak basit iki adımlı işlemden oluşur:

* *Bir yönlendirme (route) Yaratın*: Bir yönlendirme sayfanızın 
  URL'sini (Örn. ``/about``) ve yapılan istek yönlendirme deseninde
  eşleştiğinde çalıştırılacak olan controller (hangi PHP fonksiyonu ise)
  bilgisini tanımlar.

* *Bir controller yaratın*: Controller Symfony2'de gelen istekleri alan 
  ve kullanıcıya bir ``Response`` nesnesi döndüren bir PHP fonksiyonudur.

Bu basit yaklaşım Webin çalışmasıyla tam olarak eşleştiği için güzeldir.
Web üzerindeki her etkileşim bir HTTP isteği ile başlar. Uygulamanızın 
görevi basitçe isteği işleyip uygun bir HTTP cevabını döndürmektir.

Symfony2 bu fikri takip eder ve toolar ve kurallar ile size uygulamanızın
artan kullanıcı sayısı ve karmaşıklığında bile düzenli olmasını sağlar.

Yeterince basit gözüküyor değil mi? Haydi devam edelim!

.. index::
   single: Sayfa Yaratımı; Örnek

"Hello Symfony!" Sayfası
-------------------------
Haydi şimdi klasik "Hello World!" uygulamasından örneğimizi türetelim.
Örneği tamamladığınızda kullanıcı aşağıdaki URL vasıtasıyla kendi adı 
ile selamlanacak (örn. "Hello Symfony"). :

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony


Gerçekte, ``Symfony`` yazan yere kendi adınızla değiştirdiğinizde selamlama
gerçekleştirilecek. Sayfayı yaratmak için iki adımdan oluşan işlemi
takıp edin:

.. note::

    Örnekte sizin önceden Symfony2'yi indirip web sunucunuzda konfigüre 
    ettiğiniz varsayılmaktadır. Aşağıdaki URL yeni Symfony2 projenizin 
    ``localhost`` u işaret eden bir klasörde olduğunu varsayar.
    Bu işlem hakkında daha fazla bilgi almak için kullandığınız web
    sunucusunun dokümanlarını okumanız gereklidir. 
      
    Burada kullanmış olabileceğiniz web sunucularının dökümanları bulunmaktadır.
    
    * Apache HTTP Server için `Apachenin DirectoryIndex belgesi`_.
    * Nginx için  `Nginx HttpCoreModule location belgesi`_.

Başlamadan Önce: Bundle Yaratın
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Başlamadan önce bir bundle yaratmanız gerekmektedir. Symfony2'de :term:`bundle`
eklenti (plugin) gibi düşünülebilir ancak pluginden farklı olarak bundle içerisinde 
uygulamanızın bütün kodları yer alır.

Bir bundle bir dizinden çok çağırılan PHP sınıflarını, konfigürasyonlar
hatta Stil şablonları ve Javascript betiklerini gibi tüm ilgili özelliklere
ev sahipliği yapar (bkz :ref:`page-creation-bundles`).

Yaratılacak bundle'ı adı ``AcmeHelloBundle`` olacak şekilde (bu kısımda
yaratacağımız bundle'ın adı ) yaratmak için komut satırından 
şu komutların işletilmesi ve ekranda çıkacak talimatını izleyin.

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

Bu senaryonun arkasında aslında bundle'ın bulunduğu  ``src/Acme/HelloBundle``
şeklinde bir klasör yaratıldı.  Ayrıca ``app/AppKernel.php`` dosyasına 
kernel'in bundle'ı tanıması için de bir satır eklendi::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\HelloBundle\AcmeHelloBundle(),
        );
        // ...

        return $bundles;
    }

Şu anda bundle kurulumunuz var ve uygulamanızı bundle içerisinde geliştirmeye
başlayabilirsiniz.

Adım 1: Route (Yönlendirme) Yaratın
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Varsayılan olarak Symfony2 uygulamasının yönlendirme konfigürasyonu 
``app/config/routing.yml`` dosyasında bulunmaktadır. Symfony2 'deki tüm
konfigürasyonlarda olduğu gibi ayrıca farklı route konfigürasyonları
yaratmak için XML ya da PHP dosya tipleride kullanılabilir.

Eğer ana yönlendirme dosyasına bakarsanız Symfony'nin zaten yarattığınız
``AcmeHelloBundle`` 'ı eklediğini görürsünüz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        AcmeHelloBundle:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/',
        );

        return $collection;

Bu girdi oldukça kolay. Bu, Symfony'e ``AcmeHelloBundle`` içerisindeki 
``Resources/config/routing.yml`` dosyasında bulunan routing konfiürasyonunu
yüklemesini söyler.
Bunun anlamı yönlendirme konfigürasyonlarınız direkt olarak ``app/config/routing.yml``
koyabilir ya da uygulamanızdaki diğer route'ları buradan aktarabilirsimiz.

Şimdi bundle'daki ``routing.yml``  dosyasından yarattığınız sayfanın  
yeni yönlendirme URL'si aktarıldı:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Bu yönlendirme iki basit parçadan oluşur: ``pattern`` (desen) , route'un eşleştireceği
URL 'yi ve ``defaults`` adındaki array değeri, çalıştırılacak olan controller'i ifade eder.
Pattern içerisinde yer tutucu (placeholder) yazımı (``{name}``)  şeklinde ifade edilir.
Yani ``/hello/Ryan``, ``/hello/Fabien`` ya da diğer benzer URL bu route'da
eşleşecektir.  ``{name}`` Placeholder (yer tutucu) parametresi ayrıca 
controller'a aktarılacak ve siz bu değeri kullanıcıyı selamlamak amacıyla
kullanabileceksiniz.

.. note::

  Yönlendirme sistemi uygulamanıza oldukça fazla özelliği olan, esnek ve 
  güçlü bir URL sistemi özelliği verir. Bunlar hakkında daha fazla bilgi
  için  :doc:`Yönlendirme (Routing) </book/routing>` kısmını okuyun.
  

Adım 2: Controller Yaratın
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``/hello/Ryan`` adındaki bir URL uygulama tarafından ele alındığunda ``hello``
yönlendirmesi eşleşecek ve ``AcmeHelloBundle:Hello:index`` controller'i framework
tarafından çalıştırılacaktır. Sayfa yaratımının ikinci aşaması işte bu 
controller'i yaratmaktır.

``AcmeHelloBundle:Hello:index`` Controller'in *mantıksal* ismidir ve 
``Acme\HelloBundle\Controller\Hello`` adıyla çağırılan PHP sınıfındaki
``indexAction``  işaret eder. Bu dosyayı ``AcmeHelloBundle`` içerisinde
yaratarak işe başlayalım::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
    }

Gerçekte controller Symfony'nin çalıştırdığı bir PHP metodundan başka 
bir şey değildir. Bu kod sadece istekten(request) gelen bilgiyi alır ve
istenen kaynağı hazırlar. Bazı özel durumlar hariç controller her zaman
bir ``Response`` nesnesi çevirir. 

Symfony'nin ``hello`` yönlendirmesi eşleştiği zaman çalıştıracağı ``indexAction``
metodunu yaratın::

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...
    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Controller basittir. Yeni bir ``Response`` yaratır. Burada kullanılacak 
olan ilk argüman cevapta kullanacağınız içerik olmalıdır. (bu örnekte
basit bir HTML sayfası)

Tebrikler!. Sadece bir yönlendirme e controller yarattıktan sonra şu anda
elinizde tam fonksiyonlu bir sayfa var!. Eğer her şeyi doğru ayarladıysanız
uygulamanız sizi selamlamalı:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

.. tip::

    Uygulamanızı ayrıca "prod" :ref:`environment<environments-summary>`
    ortamında da şurasını ziyaret ederek görebilirsiniz:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan
    
    Eğer bir hata aldıysanız, muhtemelen ön belleğinizi temizlemeniz 
    gerekiyordur. Bunu yapmak için:
    
    .. code-block:: bash

        php app/console cache:clear --env=prod --no-debug

İsteğe bağlı ancak genel olarak sürecin üçüncü adımı bir şablon yaratmaktır.

.. note::

   Controller 'lar kodunuzun ana noktası ve sayfalarınızı yaratırken
   anahtar içeriği belirler. Bu konuda daha fazla bilgi öğrenmek için
   :doc:`Controller Bölümünü </book/controller>` okuyun.

Seçimlik Adım 3: Şablon Yaratın
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Şablonlar sayfa yerleşimi içerisindeki tüm sunacağınız şeyleri (Örn. HTML kodu)
tek bir dosya altında toplayarak tekrar kullanabilmenize olanak sağlar.
Controller içerisinde HTML kodu yazmak yerine bir şablon tasarlanır:

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

            // render a PHP template instead
            // return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
        }
    }

.. note::

   ``render()`` metodunu kullanmanıza göre controller'ınız bazı kısa 
   yolları kullanabilmek ve genel görevleri yapabilmek için 
   ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` sınıfından 
   türetilmelidir.  (API
   docs: :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`)
   Bu örnekte ``use`` belirteci ile 4.satırda eklenmiş ve 6. satırda da 
   ``Controller`` ile sınıf türetilmiştir.


``render()`` metodu bir verilen içerikle birlikte şablona aktarılacak 
``Response`` nesnesi yaratır. Diğer controllerdaki gibi en sonunda mutlaka 
``Response`` nesnesi döner.

Şablonun iki türlü ekrana basıldığını hatırlayın.
Varsayılan olarak Symfony2, iki adet farklı şablon diline izin verir; 
klasik PHP şablonları kısa ancak güçlü `Twig`_ şablonları. Endişelenmeyin,
birisini ya da ikisinide aynı projede kullanıp kullanmama seçimi size kalmış.

Controller ``AcmeHelloBundle:Hello:index.html.twig`` şablonunu şu şekildeki
isimlendirme dizilimi ile ekrana basar::

    **BundleAdi**:**ControllerAdi**:**ŞablonAdi**

Şablonun bu *mantıksal* isimi aşağıdaki fiziksel lokasyona işaret eder::

    **/path/to/BundleName**/Resources/views/**ControllerAdi**/**ŞablonAdi**

Bu durumda ``AcmeHelloBundle`` bundle ismi, ``Hello`` controller ismi 
ve ``index.html.twig`` 'de şablon olmaktadır:



.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HelloBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!


Şimdi Twig şablonunu satır satır inceleyelim:


* *satır 2*: ``extends`` ifadesi esas sablonu ifade eder. Şablon için 
  bu layout açıkça nerede konumlandıysa belirtilmelidir.

* *satır 4*: ``block`` ifadesi ``body`` olarak adlandırılan bloğun içerisinde
  çıkacak olan herşeyin burada çıkacağını ifade eder. Gördüğünüz gibi esas
  şablon (``base.html.twig``) ``body`` isimli blok ve içeriğinin ekrana basımından 
  açıkça sorumludur.

``::base.html.twig`` isimli esas şablonun **BundleAdi** ve **ControllerAdi** eksik.
(Bundan dolayı başlangıçta çift iki nokta üstüste ile (``::``) ifade ediliyor.)

Bunun anlamı esas şablon dosyasının bundle'ın dışında, ``app`` dizininde olduğunu
ifade ediyor:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Welcome!{% endblock %}</title>
                {% block stylesheets %}{% endblock %}
                <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" />
            </head>
            <body>
                {% block body %}{% endblock %}
                {% block javascripts %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Welcome!') ?></title>
                <?php $view['slots']->output('stylesheets') ?>
                <link rel="shortcut icon" href="<?php echo $view['assets']->getUrl('favicon.ico') ?>" />
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
                <?php $view['slots']->output('stylesheets') ?>
            </body>
        </html>


Ana şablon dosyası HTML planını ve ekrana basılacak olan ve ``index.html.twig``
şablonunda belirtilen ``body``  bloğunu tanımlamaktadır. Aynı zamanda yine 
``index.html.twig`` 'de tanımlanan  ``title`` bloğuda tanımlanmaktadır. 
``title`` bloğu alt şablonda tanımlanmadığında varsayılan olarak burada "Welcome!"
ifadesi yazılacaktır.

Şablonlar sayfanızdaki içeriği organize etmek ve ekrana basmak için güçlü
bir yoldur. Bir şablon HTML işaretleri CSS kodu ya da controller'in geriye
döndürdüğü yer içeriği ekrana basabilirler.

Bir isteğin işlenmesi süresince şablon motoru basit ve seçimlik bir yardımcı araçtır.
Hatırlarsanız, her controller'in ana görevi bir ``Response`` nesnesi döndürmektir.
Şablonlar güçlüdür ancak ``Response`` objeninizi yaratırken isteğe bağlı kullanacağınız
yardımcı araçlardır.

.. index::
   single: Klasör Yapısı

Klasör Yapısı
-----------------------
Bir kaç kısa bölümden sonra Symfony2'nin sayfaları yaratma ve ekrana basma
felsefesini zaten anlamış olmalısınız. Ayrıca Symfony2 projelerinin nasıl
yapılandırıldığını da gördünüz. Bu bölümün sonunda farklı tip dosyaların
nerede bulunduğunu ve bunların niçin olduğunu öğreneceksiniz.

Her ne kadar esnek olsada varsayılan olarak her Symfony :term:`uygulama` sı
aynı, önerilen klasör yapısına sahiptir.

* ``app/``: Bu klasör uygulamanın ayarlarını barındırır;

* ``src/``: Projenin tüm PHP kodu bu klasör altında tutulur;

* ``vendor/``: Her türlü sağlayıcı (vendor) kütüphaneleri burada tutulur;

* ``web/``: Bu klasör genel olarak ulaşılabilecek tüm dosyaların bulunduğu web kök klasörüdür.

Web Klasörü
~~~~~~~~~~~

Web kök klasörü resimler, stil şablonları ve javascript dosyaları gibi herkezin
erişebileceği dosyalara ev sahipliği yapar.
Aynı zamanda burada :term:`front controller` 'da bulunur::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();


Front controller dosyası, (bu örnekte ``app.php`` dosyası) Kernel sınıfını
,``AppKernel`, kullanan, görevi Symfony2 uygulamasını başlatan asıl dosyadır

.. tip::

    Bir front controllerin kullanılması demek farklı ve çok esnek URL'lerin
    basit ve düz bir PHP dosyası yerine bu dosyadan kullanılması demektir.
    Bir front controller kullanımında URL'ler aşağıdaki şekilde düzenlenir:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan
     
     Front controller,``app.php``, "içsel" olarak yönlendirme konfigürasyonundaki
     ``/hello/Ryan`` URL'sini çalıştırır.
    Apache'nin ``mod_rewrite`` kullanıldığında URL içerisinden ``app.php`` 
    dosyasını kaldırabilirsiniz.
    

    .. code-block:: text

        http://localhost/hello/Ryan

Front controller'lar temel olarak her isteği işleyebilmelerine rağmen
nadiren bunları değiştirmek hatta onları yeniden ele almak ihtiyacını 
hissedebilirsiniz. Biz bunları `Ortamlar`_ kısmında yeniden bahsedeceğiz.


Uygulama (``app``) Klasörü
~~~~~~~~~~~~~~~~~~~~~~~~~~~
Front Controller'da gördüğünüz gibi ``AppKernel`` sınıfı uygulamanın ana
noktası ve tüm konfigürasyonlardan sorumlu olan sınıftır.
Bu da ``app/`` klasöründe saklanır.

Bu sınıf mutlaka Symfony'nin uygulamanız hakkında bilmesi gereken iki 
metodu uygular. Bu metodlar için uygulama başlarken endişelenmeyin.
Symfony sizin ayarlarınıza göre bunları otomatik uygular.

* ``registerBundles()``: Uygulamanın ihtiyacı olan tüm bundle'ların bir 
  dize değişken halindeki listesi  (bkz :ref:`page-creation-bundles`);

* ``registerContainerConfiguration()``: Ana uygulama konfigürasyonu kaynak
   dosyasını yükler.(bkz `Uygulama Konfigürasyonu`_ kısmı).

Geliştirmede günden güne çok sık olarak ``app/`` dizinini, ``app/config/`` 
içerisinde bulunan konfigürasyon ve yönledirme dosyalarını değiştirmek için
kullanacaksınız. (bkz `Uygulama Konfigürasyonu`_ ). Bu klasör aynı zamanda
ön bellek dizinini (``app/cache``), bir log dizinini (``app/logs``)  ve
şablonlar gibi (``app/Resources``) uygulama-düzeyi dosyalarını ve klasör
lerini içerir.
Sonraki kısımlarda bu klasörler hakkında daha fazla bilgi öğreneceksiniz.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoloading (Otomatik Yükleme)

    
    Symfony -``app/autoload.php``  -  adındaki özel bir dosyayı hemen 
    çağırır (include eder) . Bu dosya, uygulamanızın çalışması için ihiyaç duyulan
    ``src/`` klasörü içerisinde bulunan uygulama dosyalarınızı ve ``vendor/``
    klasöründe bulunan 3. parti kütüphanelerin otomatik yüklenmesi için
    autoloader'i konfigüre eder.
    
    autoloader sayesinde herhangi bir şekilde uygulamanızda ``include`` ya da 
    ``require`` ifadelerini kullanmak zorunda kalmazsınız.
    Bunun yerine Symfony2 sınıfların namespace'lerini kullanarak bu dosyaların
    yerlerini otomatik olarak belirleyerek uygulamanıza otomatik include eder.
     
    autoloader ``src/`` klasörü içerisinde bulunan tüm PHP sınıflarını
    önceden konfigüre eder. autoloading işlemi esnasında sınıf adı ve dosyanın
    yolu aynı şekli kullanır.
     
    .. code-block:: text

        Sınıf Adı:
            Acme\HelloBundle\Controller\HelloController
        Dosya Yolu:
            src/Acme/HelloBundle/Controller/HelloController.php
            
    Temelde sadece dikkat etmeniz gereken ``app/autoload.php`` dosyası 
    içerisinde ``vendor/`` klasöründe bulunan yeni eklediğiniz 
    3. parti kütüphanelerin tanımlanmasıdır. autoloading konusunda daha fazla
    bilgi için :doc:`Sınıflar nasıl autoload edilir</components/class_loader>`  
    belgesine bakın.
     


Kaynak (``src``) Klasörü
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Basitçe ``src/`` klasörü uygulamanızı çalıştıran tüm güncel dosyalarınızın
(PHP kodu, şablonlar, konfigürasyon dosyaları, stil şablonları vs..)
bulunduğu yerdir. 
Uygulama geliştirirken çalışmanızın çok büyük ve önemli bir kısmını kapsayan
bundle'larınız  bu klasör içerisinde yaratacaksınız.

Peki gerçekten :term:`bundle` terimi neyi ifade eder ?

.. _page-creation-bundles:

Bundle Sistemi
-----------------
Bir bundle diğer yazılımlardaki plugin (eklenti) 'ye benzer ancak daha 
iyisidir. Ana farklılık, Symfony2'de çekirdek framework özellikleri ve yazdığınız
uygulamanın *tüm herşeyi* bir bundle olmasıdır.
Bundle'lar Symfony2'nin bir numaralı elemanlarıdır. Bu size önceden 
yapılandırılmış `3.parti bundle'lar`_  kullanmayı veya kendi bundle'larınızı
dağıtmak gibi esneklikler kazandırır. Aynı şekilde bu, uygulamanıza istediğiniz
özelliği seçip yüklemek ve bu özellikleri istediğiniz şekilde optimize etmeyi kolaylaştırır.

.. note::

   
   Burada temelleri öğrenirken, tüm tarif kitabı girdileri
   :doc:`bundle</cookbook/bundles/best_practices>` ların 
   organzasyonu ve en iyi örnekleri hakkında iyi bilgiler sunar.
   
Bir bundle basitçe bir özelliği hayata geçiren dosyaların düzenli bir 
şekilde bir klasörde barındırılmış halidir. Örneğin belki bir 
``BlogBundle`` yaratırsınız, belki bir ``ForumBundle`` ya da kullanıcı yönetimi
için (zaten açık kaynak kod şekilnde yaratılmış bir sürü bundle gibi )
başka bir bundle yaratırsınız. Her klasör bu özelikleri meydana getiren
PHP dosyaları, Şablonlar, stil şablonları, Javascriptler, testler ya da diğer
ne varsa, bu dosyaların tamamını içerir.

Var olan her özellik bir bundle içerisindedir ve her özellik bir bundle
ile birlikle meydana gelir.

Bundle'lardan oluşan bir uygulama  ``AppKernel`` sınıfının ``registerBundles()``
metodu ile tanımlanır::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

``registerBundles()`` methodu ile uygulamanızda kullanmak istediğiniz
(Symfony'nin çekirdek bundleları dahil) tüm bundlelar üzerinde tam kontrol
sağlarsınız.

.. tip::

   Bir bundle autoload edildiği *her yerde* çalışabilir. 
   (autoloader ``app/autoload.php`` tarafından konfigüre edilirse)
  
  
Bir Bundle Yaratmak
~~~~~~~~~~~~~~~~~

Symfony Standart Sürümü sizin için tam fonksiyonlu bundleları yaratmak
için pratik bir araç ile birlikte gelir.
Elbette bu araç olmadan da bir bundle yaratmak oldukça basittir.

Yeni bir bundle sistemi nasıl yaratılır sorusunun cevabı için ``AcmeTestBundle``
yaratıp aktif hale getirelim.

.. tip::

    Bundle ismindeki ``Acme`` kısmı tamamen uydurmadır.Bu kısmı organizasyonunuzu ifade eden
    herhangi bir "vendor" (Sağlayıcı) ismi ile değiştirebilirsiniz (Örn. ``ABC`` 
    isimli bir şirketiniz var ise ``ABCTestBundle`` olabilir.).

``src/Acme/TestBundle/`` klasöründe ``AcmeTestBundle.php`` adında yeni 
bir dosya yaratarak başlayın::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   ``AcmeTestBundle`` ismi standart :ref:`Bundle isimlendirme kuralları<bundles-naming-conventions>` 
   'na uygun olarak verilmiştir. Eğer isterseniz basitçe ``TestBundle`` sınıfınızı içeren 
   bundle'lınızın adını ``TestBundle`` olarak da seçebilirsiniz (dosyanın adlandırması ``TestBundle.php``
   olacaktır). 

Bu boş sınıf yarattığınız bundle'ın sadece bir parçası. Tamamen boş olmasına
rağmen bu sınıf bundle'ın davranışlarını düzenlemek için oldukça güçlü özelliklere
sahip.

Şimdi yarattığınız bundle'ı ``AppKernel`` sınıfı aracılığı ile aktif edelim::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // register your bundles
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

Bundan başka yapmanız gereken hiç bir şey yok. ``AcmeTestBundle`` kullanıma
hazır.

Bunun kadar kolay olarak Symfony ayrıca komut satırı arabiriminden de 
basit bir bundle sistemi iskeleti yaratmaya yarayan komutlar da içerir.

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

Bundle iskeleti basit bir controller ile şablon ve routing (yönlendirme) 
kaynakları düzenlenebilecek şekiklde yaratıldı. Symfony2'nin komut satırı
araçları hakkında daha fazla şeyi daha sonra öğreneceksiniz.

.. tip::

   
   Yeni bir bundle yaratıldığında ya da bir 3. parti bundle kullanıldığında
   mutlaka bundle ``registerBundles()`` içerisinde aktif edilmelidir.
   ``generate:bundle`` komutu kullanıldında bu sizin için otomatik olarak
   yapılır.


Bundle Klasör Yapısı
~~~~~~~~~~~~~~~~~~~~

Bir bundle'ın klasör yapısı basit ve esnektir. Varsayılan olarak bundle
sistemi, Symfony2 bundle'ları arasındaki kod tutarlılığını sağlamak amacıyla bir dizi
kuralı uygular. ``AcmeHelloBundle`` 'a bakarak bundle içerisindeki en temel
öğeleri inceleyelim:

* ``Controller/`` bundle'ın controlerlarını içerir (Örn. ``HelloController.php``);

* ``Resources/config/`` yönlendirme konfigürasyonu gibi konfigürasonları içerir. 
  (Örn. ``routing.yml``);

* ``Resources/views/`` controller ismine göre düzenlenmiş şablonları içerir.
  (Örn. ``Hello/index.html.twig``);

* ``Resources/public/`` web varlıklarını (resimler, stilşablonları, vs) 
  ve proje içerisindeki ``assets:install`` komutu ile ``web/`` klasörüne 
  kopyalanmış ya da sembolik link yaratılmış dosyaları içerir;

* ``Tests/`` bundle'ın tüm testlerini barındırır.


Bir bundle'ın basit özellikleri olabileceği gibi çok karmaşık özellikleri
de olabilir. Bir bundle sadece size gereken dosyaları barındırır. Başka bir şey
değildirler.

Kitabın ilerleyen bölümlerinde veritabanına nesnelerin nasıl yazılacağını,
formların yaratılmasını ve doğrulanmasını, uygulamanız için farklı dillere 
çevirileri, testler yazmayı vb gib pek çok şeyi göreceksiniz. Bunların her
birisi bundle içerisinde kendilerine ait olan yerlerde dururlar. 

Uygulama Konfigürasyonu
------------------------
Bir uygulama, uygulamanızın tüm özelliklerini içeren bir seri bundle 
kolleksiyonundan oluşur. Her bundle YAML, XML ya da PHP olarak yazılan
bir konfigürasyon dosyasından konfigüre edilirler. Varsayılan olarak
ana konfigürasyon dosyaları ``app/config/`` dizininde bulunur ve 
hangi formatı tercih ettiğinize bağlı olarak ``config.yml``, ``config.xml``
ya da ``config.php`` olarak adlandırılır:


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.ini }
            - { resource: security.yml }
        
        framework:
            secret:          "%secret%"
            charset:         UTF-8
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            form:            true
            csrf_protection: true
            validation:      { enable_annotations: true }
            templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
            session:
                default_locale: "%locale%"
                auto_start:     true

        # Twig Configuration
        twig:
            debug:            "%kernel.debug%"
            strict_variables: "%kernel.debug%"

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.ini" />
            <import resource="security.yml" />
        </imports>
        
        <framework:config charset="UTF-8" secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <framework:form />
            <framework:csrf-protection />
            <framework:validation annotations="true" />
            <framework:templating assets-version="SomeVersionScheme">
                <framework:engine id="twig" />
            </framework:templating>
            <framework:session default-locale="%locale%" auto-start="true" />
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.ini');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'charset'         => 'UTF-8',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'form'            => array(),
            'csrf-protection' => array(),
            'validation'      => array('annotations' => true),
            'templating'      => array(
                'engines' => array('twig'),
                #'assets_version' => "SomeVersionScheme",
            ),
            'session' => array(
                'default_locale' => "%locale%",
                'auto_start'     => true,
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   Her dosya/format 'ın nasıl yüklendiğini açık olarak sonraki kısım olan
   `Ortamlar`_ kısmında öğreneceksiniz.


``framework`` ya da ``twig`` gibi en üst düzey girdiler bir özel bir 
bundle'ı konfigüre ederler. Örneğin ``framework`` anahtarı Symfony'nin
çekirdek bundle'larından birisi olan ve yönlendirme, şablonlar ve diğer
çekirdek sistemleri içeren ``FrameworkBundle`` 'ı konfigüre eder.

Şimdi her kısım için spesifik konfigürasyon ayarları için endişelenmeyin.
Konfigürasyon dosyası varsayılan olarak iyi bir şekilde ayarlanmıştır.
Daha fazla okudukça ve Symfony2'nin diğer parçalarını da araştırdıkça
her özellik için konfigürasyon özelliklerini öğreneceksiniz.

.. sidebar:: Konfigürasyon Formatları

    Bölümler boyunca tüm konfigürasyon formatlarını 3 farklı şekilde 
    göreceksiniz (YAML, XML ve PHP). Herbirsi kendi içerisinde avantajlar
    ve dezavantajlar barındırır. Hangisini kullanacağınız konusundaki 
    seçim size kalmış:

    * *YAML*: Basit, temiz ve okunabilir;

    * *XML*: YAML'dan defalarca kuvvetli ve IDE otomatik tamamlama desteği;

    * *PHP*: Oldukça güçlü ancak standart konfigürasyon formatlarından daha az okunabilir.

.. index::
   single: Ortamlar; Giriş

.. _environments-summary:

Ortamlar
---------
Bir uygulama farklı ortamlarda çalışabilir. Farkjlı ortamlar aynı 
PHP kodunu paylaşır (front controller ayrı olarak), ancak farklı
konfigürasyonlar kullanırlar. Örneğin bir ``dev`` ortamı uyarıları ve 
hataları log altına alırken ``prod`` ortamı sadece hataları log altına alır.
Bazı dosyalar her istek durumunda ``dev`` ortamında değişirken (geliştirici
nin faydası için) fakat ``prod`` ortamında bu dosyalar önbelleklenir ve
değiştirilmez. Tüm ortamlar aynı makinede aynı uygulamayı çalıştırırlar.

Bir Symfony2 projesi gene olarak üç ortamla başlamasına rağmen (``dev``, ``test``
ve ``prod``) başka ortamlarda yaratmak oldukça kolaydır. Uygulamanızı farklı
ortamlarda görebilmek için basitçe tarayıcınızdan front controller'ınızı değiştirmeniz
yeterlidir.  Uygulamanızı ``dev`` ortamında görmek ve geliştirme front
controllerine erişmek için bu adresi tarayıcınıza vermeniz yeterlidir:


.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan


Eğer uygulamanızı ürün (production) ortamında görmek isterseniz front controller'ınızın
``prod`` ortamında çalışanını çağırmanız yeterlidir:

.. code-block:: text

    http://localhost/app.php/hello/Ryan
 
``prod`` ortamı hız için optimize edildiğinden dolayı konfigürasyon,
yonlendirme ve Twig şablonları PHP dosyaları olarak derlenir ve 
önbelleğe alınır.  Yapılan değişiklikleri  ``prod`` ortamında 
görmek istiyorsanız ön bellekleri temizleyip uygulamayı yeniden önbellekleme
yapmasını sağlamanız gereklidir:

    php app/console cache:clear --env=prod --no-debug

.. note::

   Eğer ``web/app.php`` dosyasını açarsanız basitçe ``prod`` ortamının 
   kullanılması için ayarlandığını göreceksiniz::

       $kernel = new AppKernel('prod', false);

   Yeni bir ortam için yeni bir front controller yarattığınızda bu dosyanın
   içeriği yeni front controller içerisine kopyalayıp ``prod`` değerini
   isteğinize göre değiştirebilirsiniz.

.. note::

    ``test`` ortamı tarayıcı tarafından erişimeyen otomatik test süreçleri 
    için kullanılır. Daha fazla bilgi için :doc:`Test Süreçleri Kısmına</book/testing>`
    bakınız.

.. index::
   single: Ortamlar; Konfigürasyon

Ortam Konfigürasyonları
~~~~~~~~~~~~~~~~~~~~~~~~~

``AppKernel`` sınıfı seçiminize göre güncel konfigürasyon dosyasını yüklemekten
sorumludur::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

``.yml`` uzantısını ``.xml`` ya da ``.php`` olarak değiştirebileceğinizi 
zaten biliyorsunuz. XML ya da PHP tercihinize göre konfigürasyon dosyanızı da 
düzenlemelisiniz. Her konfigürasyon tipinin kendi dosyasını yüklediğini 
unutmayın. ``dev`` ortamı için konfigürasyon dosyasına bakalım.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...


``imports`` anahtarı PHP'deki ``include`` ifadesinin aynısıdır ve görevi
ilk önce ana konfigürasyon dosyasını (``config.yml``) yüklemektir. Geri 
kalan düzenlemeler,arttırılmış loglama özellikleri ve geliştirme ortamına 
yardımcı olan diğer düzenlemelerdir. 

``prod`` ve ``test`` ortamlarının ikiside aynı modeli takip ederler. Her 
ortam temel konfigürasyon dosyasını çağırırlar (import) ve ilgili çevreye bağlı
olarak kendi konfigürasyonları içerisinde de değerleri değiştirilir. Bu sadece bir 
kuraldır ve çevreler arasında sadece gerekli parçaları değiştirerek
ana konfigrasyon değerlerini yeniden kullanmanıza olanak verir.


Özet
----
Tebrikler! Şu anda Symfony2'nin tüm temellerini ve bunların ne kadar
esnek olabileceğini gördünüz. Daha *pek çok* özellik olmasına rağmen
şimdi bazı temel noktaları aklımızda tutalım:

* sayfa yaratmak **route**, bir **controller** ve (isteğe bağlı) bir **şablon**
  yaratmayı kapsayan 3 adımlı bir süreçten oluşur.

* her proje sadece bir kaç ana klasörden oluşur: ``web/`` (web varlıkları ve
  front controllerlar), ``app/`` (konfigüasyon), ``src/`` (bundle'larınız),
  ve``vendor/`` (3. parti kodlar) (aynı zamanda vendor kütüphanelerini güncelleyecek
  araçları barındıran ``bin/`` klasörüde vardır);

* Symfony2'deki her özellik (Symfony2 framework çekirdeği de dahil) bir 
  *bundle* içerisinde organize edilir, 
* her bundle için genel **Konfigürasyon** ``app/config`` klasöründe bulunur ve
  bu YAML, XML ya da PHP tipinde olabilir,

* her **ortam** kendisine ait olan farklı front controller'lardan erişilebilir
   (Örn.``app.php`` and ``app_dev.php``)ve bu ortamlar farklı konfigürasyon 
   dosyaları yüklerler.

Buradan sonra her bölüm size çok daha fazla güçlü araç ve gelişmiş konseptler
gösterecek. Symfony2 hakkında daha fazla şey öğrendiğinizde, mimarideki esnekliğin gücünü
ve size hızlı bir geliştirme ortamı sağlamasının anlamını daha iyi anlayacaksınız.

.. _`Twig`: http://twig.sensiolabs.org
.. _`3.parti bundle'lar`: http://symfony2bundles.org/
.. _`Symfony Standard Sürümü`: http://symfony.com/download
.. _`Apachenin DirectoryIndex belgesi`: http://httpd.apache.org/docs/2.0/mod/mod_dir.html
.. _`Nginx HttpCoreModule location belgesi`: http://wiki.nginx.org/HttpCoreModule#location
