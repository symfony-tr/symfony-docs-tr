Büyük Resim
===========

Symfony2 'yi kulanmaya 10 dakikada başlayın! Bu bölüm Symfony2 'nin 
arkasındaki en önemli temelleri öğretecek ve basitçe bir projeye çabucak
nasıl başlayacağınızı anlatacaktır.

Eğer daha önceden bir framework kullandıysanız,Symfony2 ile evinizde gibi
hissedeceksiniz. Eğer öyle değilse, Web uygulamalarının yeni üretim şekline
hoş geldiniz!

.. tip::

    Neden framework kullanma ihtiyacı hissederiz sorusunu öğrenmek için 
    "`5 dakikada Symfony`_" belgesini okuyun.

Symfony2'yi İnternetten İndirmek
--------------------------------

Öncelikle bir web sunucusunun PHP 5.3.2 veya daha üstü sürümü ile kurulu
ve yapılandırılmış olduğunu kontrol edin. (Örneğin Apache gibi)

Hazır mısınız? "`Symfony2 Standard Edition`_", sürümünü indirmeye başlayın.
Symfony'deki :term:`distribution` terimi en çok kullanılan genel kullanım şeklini
barındıran ve ayrıca Symfony2'nin nasıl kullanılacağını açıklayan kodların 
oluştuğu hazır bir pakettir. (Çok hızlı bir şekilde başlamak istiyorsanız 
*vendors* paketini içeren dağıtımı indirmelisiniz. )

Paket dosyasını web sunucunuzun kök dizinine açtıktan sonra ``Symfony/`` 
adında bir dizine sahip olmalısınız. Bu dizinin içeriği aşağıdaki gibidir:

.. code-block:: text

    www/ <- sizin kök web dizininiz.
        Symfony/ <- açılmış paket
            app/
                cache/
                config/
                logs/
                Resources/
            bin/
            src/
                Acme/
                    DemoBundle/
                        Controller/
                        Resources/
                        ...
            vendor/
                symfony/
                doctrine/
                ...
            web/
                app.php
                ...

.. note::

    Eğer standart sürümün *without vendors*, şeklini indirdiyseniz, basitçe 
    şu kodu çalıştırarak gerekli tüm vendor yazılımlarını indirebilirsiniz :

    .. code-block:: bash

        php bin/vendors install

Konfigürasyon Kontrolü
-----------------------
Symfony2 Web sunucunuzun ya da PHP'nin yanlış ayarlanması dolayısıyla 
oluşacak baş ağrılarından kurtaracak görsel bir konfigürasyon test aracı 
ile birlikte gelir. Makinanızı şu URL üzerinden test edebilirsiniz:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Eğer hatalı olarak listelenen bir kısım varsa onları düzeltin.Ayrıca ayarlarınızı
size verilen bazı ip uçları ile daha kullanışlı hale getirebilirsiniz.Herşey
yolunda olduğu zaman "*Bypass configuration and go to the Welcome page*" linkine
tıklayarak ilk *gerçek* Symfony2 web sayfasına gidebilirsiniz :

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/
Symfony2 sizin bu sıkı çalışmanızı tebrik ederek size Hoşgeldiniz diyecektir.!

.. image:: /images/quick_tour/welcome.jpg
   :align: center

Temelleri Anlamak
-----------------

Bir framework'un ana amaçlarından bir tanesi şunu sağlamaktır. 
`İşleri Ayırmak`_ .
Frameworkler kodunuzu organize eder, veri tabanı çağrıları ile harmanlanmış
HTML taglarından oluşan çıktıları kolaylıkla aynı betik üzerinde geliştirmenizi sağlar.
Bunu Symfony2 ile başarabilirsiniz, ancak bazı temel kavramları öğrenmeniz
gereklidir:

.. tip::

    Bir framework'un kullanılmasının herşeyin aynı betik üzerinde karmaşık
    bir şekilde kullanılmasından daha iyi olduğunun kanıtını görmek istiyorsanız,
    kitabın     ":doc:`/book/from_flat_php_to_symfony2`" bölümünü okuyun.

Dağıtım Symfony2 temel yapılarını daha iyi anlatmak açısından basit bir kod ile 
birlikte gelir. Şu  URL 'ye giderek Symfony2'nin sizi selamlamasını sağlayın.
(*Fabien* yerine kendi isiminizi koyun):

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/demo/hello/Fabien

.. image:: /images/quick_tour/hello_fabien.png
   :align: center

Burada şimdi ne oldu ? URL 'yi inceleyelim :

* ``app_dev.php``: Bu :term:`front controller`. Uygulamada kullanıcının 
tüm isteklerine cevap veren tek yer.;

* ``/demo/hello/Fabien``: Bu *virtual path* kullanıcının erişmek istediği yer.

Geliştirici olarak sorumluluğunuz kullanıcıların isteklerini 
(*request* (``/demo/hello/Fabien``) ) kaynaklarla (*resource*) )
(``Hello Fabien!`` HTML sayfası) birleştirmektir.

Yönlendirme (Routing)
~~~~~~~~~~~~~~~~~~~~~

Symfony2 kod üzerinde önceden belirlenmiş URL pattern (desenleri) ile 
kullanıcının istek (request) 'lerini eşleştirmeye çalışır.

Varsayılan olarak bu desenler(pattern) (yönlendirme olarak adlandırılır)
``app/config/routing.yml`` dosyasında tanımlanmıştır.
Eğer siz  app_**dev**.php front controller 'nın işaret ettiği 
``dev`` :ref:`ortamındaysanız<quick-tour-big-picture-environments>` 
``app/config/routing_dev.yml`` dosyası yüklenir. Standart Sürümdeki 
demoda yönlendirmeler şu dosyadadır:

.. code-block:: yaml

    # app/config/routing_dev.yml
    _welcome:
        pattern:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

    # ...

İlk üç satır (yorum satırlarından sonra) kullanıcının "``/``" isteğinde 
ilgili kaynağı çalıştırır.(Örn. Önceden gördüğünüz Wellcome ekranı). 
``AcmeDemoBundle:Welcome:index`` controller'ının isteği yapıldığında bu çalışacaktır.
Sonraki bölümde bunun ne manaya geldiğini daha ayrıntılı olarak göreceksiniz.

.. tip::

    Symfony2 standart sürümü konfigürasyon dosyaları için `YAML`_ kullanır,
    fakat Symfony2 aynı zamanda XML, PHP ve doğal alıntılarıda kullanabilir.
	Bu farklı formatlar uygulamada bir birlerinin yerine kullanılabilir.
	Fakat bu farklı formatların kullanılması uygulamanızın performansına bir
	etki etmez.Çünki herşey daha ilk istekte ön belleğe zaten alınmaktadır.

Controller'lar
~~~~~~~~~~~~~~

Controller, gelen istek *request* leri cevap *response* (genellikle HTML kodu) 
olarak çeviren bir PHP fonksiyonu ya da metodunun fantastik bir ismidir. 

PHP global değişkenleri ya da fonksiyonlarını (``$_GET`` ya da  ``header()`` gibi)
kullanmak yerine Bu HTTP mesajlarını Symfony2 
:class:`Symfony\\Component\\HttpFoundation\\Request`
ve :class:`Symfony\\Component\\HttpFoundation\\Response` sınıflarını 
kullanarak yönetir.

Gelen isteği cevaplandırabilecek mümkün olan en basit controller yapısı 
şu şekildedir::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Symfony2 tüm web iletişimini sağlayan HTTP kurallarının tamamını destekler.
    Kitabın ":doc:`/book/http_fundamentals`" bölümünü okuyarak bu konuda daha çok 
    bilgi alabilir ve bunun getirdiği gücü kullanabilirsiniz.

Symfony2 yönlendirme konfigürasyonunda  ``_controller`` altında verilen
``AcmeDemoBundle:Welcome:index`` değerini controller olarak seçer. 
Bu ifade controller'ın mantıksal adı *logical name* 'dır ve 
``Acme\DemoBundle\Controller\WelcomeController`` sınıfının içerisindeki 
``indexAction`` metoduna işaret eder ::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    Yönlendirme konfigürasyonunda bulunan  ``_controller`` değerine aynı 
    zamanda  ``Acme\DemoBundle\Controller\WelcomeController::indexAction`` 
    şeklinde tam sınıf ve metod adı da kullanabilirsiniz. Ancak genel olarak
    uygulamada mantıksal isimler daha kısa olduğu için bu size daha fazla
    esneklik sağlar.


``WelcomeController`` sınıfı (``AcmeDemoBundle:Welcome:index.html.twig``) şablonunu yükleyip ekrana basan 
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render` methodu gibi kullanışlı kısayol 
metodlarını sağlayan Symfony2'nin yerleşik sınıflarından olan ``Controller`` sınıfından türetilir.

Geri dönen değer işlenen içeriği toplayan bir Response nesnesidir. Eğer Response nesnesini tarayıcıya
göndermeden önce  bazı düzenlemeler yapmak isterseniz ::

    public function indexAction()
    {
        $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

şeklinde kullanmalısınız. Ne şekilde yaptığınız önemli değil. Kontroller'ınızın ana amacı
her zaman ``Response`` nesnesini geri döndürmektir. Bu ``Response`` nesnesi HTML kodunu
oluşturabilir iken aynı zamanda bir yeniden yönlendirme (redirect) işleminide yapabilir ya da
``Content-Type`` başlığında ``image/jpg`` ayarlandığı zaman bir JPG içeriğide döndürebilir.

.. tip::

   ``Controller`` ana sınıgını genişletme işi tercihandır. Aslında bir controller,
   basit bir PHP fonksiyonuda olabilir bir PHP kapatma fonksiyonuda olabilir (closure).
   ":doc:`The Controller</book/controller>`" bölümü Symfony2 Controller'ları hakkındaki
   herşeyi anlatmaktadır.

``AcmeDemoBundle:Welcome:index.html.twig`` isimli şablon adı 
``AcmeDemoBundle`` (``src/Acme/DemoBundle`` da bulunan)
içerisindeki ``Resources/views/Welcome/index.html.twig`` 
dosyasını işaret eden mantıksal bir addır.
 
Bunun neden çok kullanışlı olduğu Bundle kısmında açıklanacaktır.

Şimdi yeniden yönlendirme (routing) konfigürasyonuna bakalım ve ``_demo``
anahtarını bulalım :

.. code-block:: yaml

    # app/config/routing_dev.yml
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo


Symfony2 YAML, XML, PHP  olarak farklı kaynaklardaki dosyalardan 
ya da PHP kodunun içerisine gömülmüş Annotation (belirteç)
lardan yönlendirme ayarlarını okuyabilir / içeri aktarabilir (import).

Burada ``@AcmeDemoBundle/Controller/DemoController.php`` dosyasının
*mantıksal* (logical) isimi ile işaret ettiği 
``src/Acme/DemoBundle/Controller/DemoController.php``
dosyası verilmiştir. Bu dosyadada yönlendirmeler action metodları 
için yönlendirmeler belirteç(Annotation) olarak verilmiştir:: 

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    class DemoController extends Controller
    {
        /**
         * @Route("/hello/{name}", name="_demo_hello")
         * @Template()
         */
        public function helloAction($name)
        {
            return array('name' => $name);
        }

        // ...
    }

``@Route()`` belirteci  ``/hello/{name}`` patterni (deseni) ile 
``helloAction`` metodunu ile eşleşen bir kodu çalıştıran bir yönlendirmeyi 
temsil eder. ``{name}`` şeklinde küme parantezleri arasında ifade edilen 
bu değişken yertutucu (placeholder) olarak ifade edilir. Görebildiğiniz gibi
metodun ``$name`` argümanını işaret eder.

.. note::

    Belirteçler (annotation) PHP tarafından doğal (native) olarak desteklenmez.
    Symfony2 belirteçleri framework davranışlarını daha basit bir şekilde konfigüre
    etmek ve sonraki koda aktarmak için kullanır.

Controller koduna daha yakından bakarsanız,görebileceğiniz gibi şablonu render etmek için
önceden ``Response`` nesnesi kullanılmuştı. Şimdi ise sadece bir dize (array) döndürülmekte.
``@Template()`` belirteci Symfony'ye controller'ın return kısmındaki array içerisinde 
belirtilen her değeri şablona aktarmasını söyler. Şablonun adı ilgili controller'ın adıdır. 
Bu yüzden bu örnekte ``AcmeDemoBundle:Demo:hello.html.twig`` şablonu render edilmiştir. 

.. tip::

    ``@Route()`` ve ``@Template()`` belirteçleri bu öğreticide gösterilen örneklerdekinden
    çok daha fazlasını yapabilir. Bunun için resmi dokümanlardaki "`controller içinde belirteçler`_" başlıklı bölümü
    okuyun.

Şablonlar
~~~~~~~~~

Controller,  
``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig`` şablonunu render eder 
(ya da eğer mantıksal ad kullandıysanız ``AcmeDemoBundle:Demo:hello.html.twig``):

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Symfony2 varsayılan şablon motoru olarak Twig'i kullabildiği gibi eğer isterseniz
geleneksel PHP şablonlarını da kullabilirsiniz. Sonraki bölümde şablonların Symfony2'de
nasıl kullanılacağına bir giriş yapılacaktır.

Bundle'lar
~~~~~~~~~~

:term:`bundle` kelimesi neden çok fazla kullandığımızı merak etmiş olabilirsiniz.
Uygulamanızı oluşturan yazdığınız tüm kodlar bundle yapısı içerisinde organize olurlar.

Symfony2'de bunde'lar geliştiricilerle tek özelliği olan (bir blog, bir forum) ve bu iş için pek
çok dosyadan oluşan (PHP dosyaları, stil şablonları, Javascriptler, resimler...) bir 
yapıda ortak bir dilde konuşur.

Bundan sonra ``AcmeDemoBundle`` adlı bir bundle ile çalışıyoruz. 
Bundle'lar hakkında daha fazla bilgiyi bu öğretiinin son bölümünde öğreneceksiniz.


.. _quick-tour-big-picture-environments:

Environments (Ortamlar)
-----------------------
-----------------------

Symfony2'nin çıktı sayfasına daha dikkatli bakarsanız Symfony2'nin nasıl
çalıştığını daha iyi anlayabilirsiniz.Symfony2 logosunun altındaki küçük
bara dikkat edin. Bu "Web Debug Araç Çubuğu" olarak geçer ve geliştiricilerin
en iyi arkadaşıdır.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center

Fakat bu sadece buzdağının görünen yüzü yukarıdaki bu acayip 16'lık (hex
adecimal) koda tıkladığınız zaman karşınıza Symfony2'nin başka bir 
kullanışlı aracı olan profiler gelecektir.

.. image:: /images/quick_tour/profiler.png
   :align: center

Elbette uygulamamızın nihai halinde bu araçları göstermek istemeyiz. Buda  
``web/`` klasöründeki (``app.php``) adındaki nihai ugulamanın front controller
dosyasının varlığını açıklar.

.. code-block:: text

    http://localhost/Symfony/web/app.php/demo/hello/Fabien

Ve eğer apache suncusu üzerinde ``mod_rewrite`` açıksa URL satırından 
``app.php`` kısmını da çıkartabilirsiniz:

.. code-block:: text

    http://localhost/Symfony/web/demo/hello/Fabien

Son olarak, uygulamanın çalışacağı sunucuda , web kök klasörünü ``web/`` 
gizleyecek şekilde ayarlayarak daha düzgün bir URL gösterebilirsiniz:

.. code-block:: text

    http://localhost/demo/hello/Fabien

.. note::

    Bu üç URL adresinin sadece URL adreslerini uygulamanın
    çalışacağı sunucuda nasıl görüleceğini (mod_rewrite ile ya da 
    mod_rewrite olmadan) **örneklemek** amacıyla verildiğini unutmayın.
    Eğer *Symfony Standart Sürüm* 'ü uzaktaki sunucuya kurduysanız
    *AcmeDemoBundle* sadece dev ortamında çalıştığı ve yönlendirmeleri 
    *app/config/routing_dev.yml* dosyasından aldığı için 404 
    hatası alabilirsiniz.


Uygulamanızın daha hızlı cevap vermesi için Symfony2 ``app/cache/`` dizini
altında ön bellekleme yapar. Geliştirme ortamında (``app_dev.php``) yapılan
her kod ya da konfigürasyon değişikliğinde bu ön bellek otomatik olarak 
boşaltılır. Ancak uygulama ortamında (``app.php``) performans ana unsur
olduğu için bu yapılmaz. Buda neden geliştirme ortamında çalışmanız 
gerektiğini açıklar.

Verilen uygulamanın farklı :term:`ortamları<environment>` sadece kendi
konfigürasyonlarında ayrılırlar. Aslında bir konfigürasyon diğer birinden
kalıtım sağlayabilir:

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

``dev`` çevresi (``config_dev.yml konfigürasyon dosyasını yükler) genel
``config.yml`` dosyasını içeri aktarır (import) ve onu bu örnekte web debug toolbar
'ı açık olacak şekilde değiştirir.

Son Söz
-------

Tebrikler!  Symfony2 kodundaki ilk lezzeti tattınız. O kadar zor değil değil mi ?
Daha çok araştırılacak şey var ancak Symfony2'nin nasıl web sitelerini çabuk
ve hızlı bir şekilde geliştirdiğini gördünüz. Eğer Symfony2 hakkında daha fazlasını
öğrenmeye meraklı iseniz, sonraki ":doc:`Görünüm (view)<the_view>`" bölümüne dalabilirsiniz.


.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _5 dakikada Symfony:             http://symfony.com/symfony-in-five-minutes
.. _İşleri ayırmak:         http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _controller içinde belirteçler:  http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/index.html#annotations-for-controllers
.. _Twig:                           http://twig.sensiolabs.org/
