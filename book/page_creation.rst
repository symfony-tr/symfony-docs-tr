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
ve ``index.html.twig`` 'de şablon olmaktadır::



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


Front controller dosyası (bu örnekte ``app.php`` dosyası) Kernel sınıfını
,``AppKernel`, kullanan görevi Symfony2 uygulamasını başlatan asıl dosyadır

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

        Class Name:
            Acme\HelloBundle\Controller\HelloController
        Path:
            src/Acme/HelloBundle/Controller/HelloController.php

	Temelde sadece dikkat etmeniz gereken ``app/autoload.php`` dosyası 
	içerisinde ``vendor/`` klasöründe bulunan yeni eklediğiniz 
	3. parti kütüphanelerin tanımlanmasıdır. autoloading konusunda daha fazla
	bilgi için  :doc:`Sınıflar nasıl autoload edilir ?</components/class_loader>` belgesine
	bakın.
	
   

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
yapılandırılmış `3.parti bundle'lar_`  kullanmayı veya kendi bundle'larınızı
dağıtmak gibi esneklikler kazandırır. Aynı şekilde bu, uygulamanıza istediğiniz
özelliği seçip yüklemek ve bu özellikleri istediğiniz şekilde optimize etmeyi kolaylaştırır.

.. note::

   
   Burada temelleri öğrenirken, tüm tarif kitabı girdileri
   :doc:`bundle</cookbook/bundles/best_practices>`ların 
   organzasyonu ve en iyi örnekleri hakkında iyi bilgiler sunar.
   
Bir bundle basitçe bir özelliği hayata geçiren dosyaların düzenli bir 
şekilde bir klasörde barındırılmış halidir. Örneğin belki bir 
``BlogBundle`` yaratırsınız, belki bir ``ForumBundle`` ya da kullanıcı yönetimi
için (zaten açık kaynak kod şekilnde yaratılmış bir sürü bundle gibi )
başka bir bundle yaratırsınız. Her klasör bu özelikleri meydana getiren
PHP dosyaları, Şablonlar, stil şablonları, Javascriptler, testler ya da diğer
ne varsa, bu dosyaların tamamını içerir.


A bundle is simply a structured set of files within a directory that implement
a single feature. You might create a ``BlogBundle``, a ``ForumBundle`` or
a bundle for user management (many of these exist already as open source
bundles). Each directory contains everything related to that feature, including
PHP files, templates, stylesheets, JavaScripts, tests and anything else.
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
  

Creating a Bundle
~~~~~~~~~~~~~~~~~

The Symfony Standard Edition comes with a handy task that creates a fully-functional
bundle for you. Of course, creating a bundle by hand is pretty easy as well.

To show you how simple the bundle system is, create a new bundle called
``AcmeTestBundle`` and enable it.

.. tip::

    The ``Acme`` portion is just a dummy name that should be replaced by
    some "vendor" name that represents you or your organization (e.g. ``ABCTestBundle``
    for some company named ``ABC``).

Start by creating a ``src/Acme/TestBundle/`` directory and adding a new file
called ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   The name ``AcmeTestBundle`` follows the standard :ref:`Bundle naming conventions<bundles-naming-conventions>`.
   You could also choose to shorten the name of the bundle to simply ``TestBundle``
   by naming this class ``TestBundle`` (and naming the file ``TestBundle.php``).

This empty class is the only piece you need to create the new bundle. Though
commonly empty, this class is powerful and can be used to customize the behavior
of the bundle.

Now that you've created the bundle, enable it via the ``AppKernel`` class::

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

And while it doesn't do anything yet, ``AcmeTestBundle`` is now ready to
be used.

And as easy as this is, Symfony also provides a command-line interface for
generating a basic bundle skeleton:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

The bundle skeleton generates with a basic controller, template and routing
resource that can be customized. You'll learn more about Symfony2's command-line
tools later.

.. tip::

   Whenever creating a new bundle or using a third-party bundle, always make
   sure the bundle has been enabled in ``registerBundles()``. When using
   the ``generate:bundle`` command, this is done for you.

Bundle Directory Structure
~~~~~~~~~~~~~~~~~~~~~~~~~~

The directory structure of a bundle is simple and flexible. By default, the
bundle system follows a set of conventions that help to keep code consistent
between all Symfony2 bundles. Take a look at ``AcmeHelloBundle``, as it contains
some of the most common elements of a bundle:

* ``Controller/`` contains the controllers of the bundle (e.g. ``HelloController.php``);

* ``Resources/config/`` houses configuration, including routing configuration
  (e.g. ``routing.yml``);

* ``Resources/views/`` holds templates organized by controller name (e.g.
  ``Hello/index.html.twig``);

* ``Resources/public/`` contains web assets (images, stylesheets, etc) and is
  copied or symbolically linked into the project ``web/`` directory via
  the ``assets:install`` console command;

* ``Tests/`` holds all tests for the bundle.

A bundle can be as small or large as the feature it implements. It contains
only the files you need and nothing else.

As you move through the book, you'll learn how to persist objects to a database,
create and validate forms, create translations for your application, write
tests and much more. Each of these has their own place and role within the
bundle.

Application Configuration
-------------------------

An application consists of a collection of bundles representing all of the
features and capabilities of your application. Each bundle can be customized
via configuration files written in YAML, XML or PHP. By default, the main
configuration file lives in the ``app/config/`` directory and is called
either ``config.yml``, ``config.xml`` or ``config.php`` depending on which
format you prefer:

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

   You'll learn exactly how to load each file/format in the next section
   `Environments`_.

Each top-level entry like ``framework`` or ``twig`` defines the configuration
for a particular bundle. For example, the ``framework`` key defines the configuration
for the core Symfony ``FrameworkBundle`` and includes configuration for the
routing, templating, and other core systems.

For now, don't worry about the specific configuration options in each section.
The configuration file ships with sensible defaults. As you read more and
explore each part of Symfony2, you'll learn about the specific configuration
options of each feature.

.. sidebar:: Configuration Formats

    Throughout the chapters, all configuration examples will be shown in all
    three formats (YAML, XML and PHP). Each has its own advantages and
    disadvantages. The choice of which to use is up to you:

    * *YAML*: Simple, clean and readable;

    * *XML*: More powerful than YAML at times and supports IDE autocompletion;

    * *PHP*: Very powerful but less readable than standard configuration formats.

.. index::
   single: Environments; Introduction

.. _environments-summary:

Environments
------------

An application can run in various environments. The different environments
share the same PHP code (apart from the front controller), but use different
configuration. For instance, a ``dev`` environment will log warnings and
errors, while a ``prod`` environment will only log errors. Some files are
rebuilt on each request in the ``dev`` environment (for the developer's convenience),
but cached in the ``prod`` environment. All environments live together on
the same machine and execute the same application.

A Symfony2 project generally begins with three environments (``dev``, ``test``
and ``prod``), though creating new environments is easy. You can view your
application in different environments simply by changing the front controller
in your browser. To see the application in the ``dev`` environment, access
the application via the development front controller:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

If you'd like to see how your application will behave in the production environment,
call the ``prod`` front controller instead:

.. code-block:: text

    http://localhost/app.php/hello/Ryan

Since the ``prod`` environment is optimized for speed; the configuration,
routing and Twig templates are compiled into flat PHP classes and cached.
When viewing changes in the ``prod`` environment, you'll need to clear these
cached files and allow them to rebuild::

    php app/console cache:clear --env=prod --no-debug

.. note::

   If you open the ``web/app.php`` file, you'll find that it's configured explicitly
   to use the ``prod`` environment::

       $kernel = new AppKernel('prod', false);

   You can create a new front controller for a new environment by copying
   this file and changing ``prod`` to some other value.

.. note::

    The ``test`` environment is used when running automated tests and cannot
    be accessed directly through the browser. See the :doc:`testing chapter</book/testing>`
    for more details.

.. index::
   single: Ortamlar; Configuration

Environment Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~

The ``AppKernel`` class is responsible for actually loading the configuration
file of your choice::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

You already know that the ``.yml`` extension can be changed to ``.xml`` or
``.php`` if you prefer to use either XML or PHP to write your configuration.
Notice also that each environment loads its own configuration file. Consider
the configuration file for the ``dev`` environment.

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

The ``imports`` key is similar to a PHP ``include`` statement and guarantees
that the main configuration file (``config.yml``) is loaded first. The rest
of the file tweaks the default configuration for increased logging and other
settings conducive to a development environment.

Both the ``prod`` and ``test`` environments follow the same model: each environment
imports the base configuration file and then modifies its configuration values
to fit the needs of the specific environment. This is just a convention,
but one that allows you to reuse most of your configuration and customize
just pieces of it between environments.

Summary
-------

Congratulations! You've now seen every fundamental aspect of Symfony2 and have
hopefully discovered how easy and flexible it can be. And while there are
*a lot* of features still to come, be sure to keep the following basic points
in mind:

* creating a page is a three-step process involving a **route**, a **controller**
  and (optionally) a **template**.

* each project contains just a few main directories: ``web/`` (web assets and
  the front controllers), ``app/`` (configuration), ``src/`` (your bundles),
  and ``vendor/`` (third-party code) (there's also a ``bin/`` directory that's
  used to help updated vendor libraries);

* each feature in Symfony2 (including the Symfony2 framework core) is organized
  into a *bundle*, which is a structured set of files for that feature;

* the **configuration** for each bundle lives in the ``app/config`` directory
  and can be specified in YAML, XML or PHP;

* each **environment** is accessible via a different front controller (e.g.
  ``app.php`` and ``app_dev.php``) and loads a different configuration file.

From here, each chapter will introduce you to more and more powerful tools
and advanced concepts. The more you know about Symfony2, the more you'll
appreciate the flexibility of its architecture and the power it gives you
to rapidly develop applications.

.. _`Twig`: http://twig.sensiolabs.org
.. _`third-party bundles`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`Apachenin DirectoryIndex belgesi`: http://httpd.apache.org/docs/2.0/mod/mod_dir.html
.. _`Nginx HttpCoreModule location belgesi`: http://wiki.nginx.org/HttpCoreModule#location
