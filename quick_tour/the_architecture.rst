Mimari
================

Sen Benim Kahramanımsın! Acaba bu üç kelimenin burada ne işi olduğunu kim
düşünür.? Çabalarınızın ödülünü daha sonra alacaksınız. Bu üç kelime aslında
framework'un derinlemesine tarifine yetmez. Çünkü Symfony2 kendisini diğer
framework kalabalıklığından uzak tutar. Şimdi mimariye bakalım.

Dizin Yapısını Anlamak
-------------------------------------

Symfony2  :term:`uygulaması` dizin yapısı oldukça esnek olmasına rağmen,
*Standard Sürüm* dağıtımının dizin yapısı Symfony2 önerilen tipik uygulama dizin
yapısını yansıtmaktadır.:

* ``app/``:    Uygulama konfigürasyonları;
* ``src/``:    Projenin PHP kodu;
* ``vendor/``: 3. parti bağımlılıklar;
* ``web/``:    web kök dizini.

``web/`` Dizini
~~~~~~~~~~~~~~~~~~~~~~

Web kök dizini stil şablonları, javascript dosyaları resimler gibi statik
dosyaların varolduğu ana yerdir. Aynı zamanda bu dizinde :term:`front controller`
dosyalarıda bulunur::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();


Çekirdek öncelikle ``bootstrap.php.cache`` dosyasına istek yaparak frameworkü
yükler ve autoloader'daki leri register eder (aşağıda gösterilmiştir).

Diğer front controller'lar gibi ``app.php`` , Kernel Sınıfını ``AppKernel``
uygulamayı başlatmak için kullanır. 

.. _the-app-dir:

``app/`` Dizini
~~~~~~~~~~~~~~~~~~~~~~

``AppKernel`` sınıfı uygulama konfigürasyonlarının tutuldupğu ``app/`` 
dizininin ana giriş noktasıdır.

Bu sınıfın mutlaka şu iki metodu uygulanmalıdır:

* ``registerBundles()`` uygulamanın çalışması için gerekli tüm bundle'ları
  array (dize) olarak döndürmelidir.

* ``registerContainerConfiguration()`` tüm uygulama konfigürasyonlarını yükler.
  (Bu daha sonra açıklanacaktır).

PHP autoloading ``app/autoload.php`` tarafından konfigüre edilebilir ::

    // app/autoload.php
    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony'          => array(__DIR__.'/../vendor/symfony/src', __DIR__.'/../vendor/bundles'),
        'Sensio'           => __DIR__.'/../vendor/bundles',
        'JMS'              => __DIR__.'/../vendor/bundles',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        'Doctrine\\DBAL'   => __DIR__.'/../vendor/doctrine-dbal/lib',
        'Doctrine'         => __DIR__.'/../vendor/doctrine/lib',
        'Monolog'          => __DIR__.'/../vendor/monolog/src',
        'Assetic'          => __DIR__.'/../vendor/assetic/src',
        'Metadata'         => __DIR__.'/../vendor/metadata/src',
    ));
    $loader->registerPrefixes(array(
        'Twig_Extensions_' => __DIR__.'/../vendor/twig-extensions/lib',
        'Twig_'            => __DIR__.'/../vendor/twig/lib',
    ));

    // ...

    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));
    $loader->register();

:class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` sınıfı PHP
5.3 namespace'leri yüklemek için kullandığı `standartları`_ sağlamak ya da PEAR
`adlandırma`_ kurallarını teknik olarak bir çatıda birleştirmek için kullanılır. 

Görebildiğimniz gibi tüm bağımlılıklar ``vendor/`` dizini altındadır ancak 
bu sadece bir kuraldır. Eğer isterseniz bu bağımlılıkları başka bir dizinde de
saklamanız mümkündür.

.. note::

    Eğer Symfony2 autloader'ının esnekliği konusunda daha fazla bilgiye
    sahip olmak isterseniz ":doc:`/components/class_loader`" bölümünü okuyun.
    
Bundle Sistemini Anlamak
-------------------------------
Bu kısımda Symfony2'nin en önemli ve güçlü özelliklerinden birisi olan 
:term:`bundle` sistemine bir giriş yapılacaktır.

Bir bundle diğer yazılımdardaki plug-in'ler gibidir. Peki biz neden *plug-in*
yerine *bundle* diyoruz?. Çünki framework çekirdeğinden yazdığınız uygulama
kodlarına kadar *herşey* Symfony2'de bundle'lar içerisindedir.Bundle'lar 
Symfony2'nin birinci sınıf vatandaşıdır. Bu özellik size 3. parti bundlelar
içerisindeki ön yapılandırılmış özellikleri kullanmanıza kendi bundle'ınızı
dağıtmanıza ve başka yerlerde kullanmanıza kadar çeşitli esneklikler verir.

Uygulamanızda neleri kullanacağınıza karar vermek ve hangi özellikleri aktif etmek
istediğinizi belirlemek bu şekilde oldukça kolaylaşacaktır.
Ve günün sonunda uygulamanızın kodu en az framework çekirdeği kadar *önemli*
hale gelecektir.

Bundle'ı Kayıtlamak (Register)
~~~~~~~~~~~~~~~~~~~~
Bir uygulamada kullanılacak olan bundle'lar ``AppKernel`` sınıfının 
``registerBundles()`` metodu ile tanımlanırlar Her bundle kendisini tanımlayan
bir ``Bundle`` sınıfını içeren bir klasör'den oluşur::


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

``AcmeDemoBundle`` 'ın ne olduğunu önceden konuşmuştuk. Buna ek olarak 
çekirdek ayrıca ``FrameworkBundle``, ``DoctrineBundle``, ``SwiftmailerBundle``, 
ve ``AsseticBundle`` adlı bundlelara ihtiyaç duymaktadır. Bunların tamamı 
çekirdek framework'e gerekli olan bundle'lardır. 

Bundle Konfigürasyonu
~~~~~~~~~~~~~~~~~~~~
Her bundle YAML, XML ya da PHP olan dosyalar ile konfigüre edilebilir.
Şimdi varsayılan ayarlara bakalım:

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

    # Assetic Configuration
    assetic:
        debug:          "%kernel.debug%"
        use_controller: false
        filters:
            cssrewrite: ~
            # closure:
            #     jar: "%kernel.root_dir%/java/compiler.jar"
            # yui_css:
            #     jar: "%kernel.root_dir%/java/yuicompressor-2.4.2.jar"

    # Doctrine Configuration
    doctrine:
        dbal:
            driver:   "%database_driver%"
            host:     "%database_host%"
            dbname:   "%database_name%"
            user:     "%database_user%"
            password: "%database_password%"
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: "%kernel.debug%"
            auto_mapping: true

    # Swiftmailer Configuration
    swiftmailer:
        transport: "%mailer_transport%"
        host:      "%mailer_host%"
        username:  "%mailer_user%"
        password:  "%mailer_password%"

    jms_security_extra:
        secure_controllers:  true
        secure_all_services: false

``framework`` gibi tanımlan her girdi aslında özel bir bundle'ın konfigürasyonunu
ifade eder. Örneğin ``framework`` , ``FrameworkBundle`` 'ı konfigüre ederken 
``swiftmailer``, ``SwiftmailerBundle`` 'ı konfigüre eder.


Her :term:`ortam` özel bir konfigürasyon dosyasından aktarılarak konfigüre edilebilir. 
Örneğin ``dev`` ortamı aslında ana konfigürasyon dosyasını çağıran ancak bazı hata ayıklama
ve yardımcı araçların konfigürasyonularını da  içeren ``config_dev.yml`` dosyasından
konfigüre edilir.


.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    framework:
        router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
        profiler: { only_exceptions: false }

    web_profiler:
        toolbar: true
        intercept_redirects: false

    monolog:
        handlers:
            main:
                type:  stream
                path:  "%kernel.logs_dir%/%kernel.environment%.log"
                level: debug
            firephp:
                type:  firephp
                level: info

    assetic:
        use_controller: true

Bundle'ı Genişletmek
~~~~~~~~~~~~~~~~~~

Bir bundle kodunuzu organize etmek ve konfigüre edebilmek için güzel bir
yol olabileceği gibi başka bir bundle ile bundle'ınızı genişletebilirsiniz.

Bundle'ları  miras almaları size başka bir bundleın controller'larını, şablonlarını
ya da diğer dosyalarını kumanda etmenize olanak sağlar. 

Bunun için mantıksal isimler kullanmak (Örn: ``@AcmeDemoBundle/Controller/SecuredController.php``)
saklanan ulaşmak istediğiniz kaynağa gitmek için oldukça elverişli bir yoldur.

Mantıksal Dosya İsimleri
.........................
Bundle dan ne zaman bir dosyayı işaret etmek isterseniz ``@BUNDLE_NAME/path/to/file`` 
belirtme şeklini kullanabilirsiniz.

Bu belirtme şeklinde Symfony2 bundle'un gerçek yolunu ``@BUNDLE_NAME`` kısmından 
çözecektir. Örneğin ``@AcmeDemoBundle/Controller/DemoController.php` şeklindeki
mantıksal bir dosya ismi ``src/Acme/DemoBundle/Controller/DemoController.php``
şekline çevrilecektir. Çünkü Symfony ``AcmeDemoBundle`` 'ın yerini bilmektedir.

Mantıksal Controller İsimleri
.............................
Controller'ların metodlarına işaret etme için 
``BUNDLE_NAME:CONTROLLER_NAME:ACTION_NAME`` şekli kullanılır.
Örneğin ``AcmeDemoBundle:Welcome:index`` , ``Acme\DemoBundle\Controller\WelcomeController`` 
sınıfının ``indexAction`` metoduna işaret eder.


Mantıksal Şablon İsimleri
.........................

Şablonların ``AcmeDemoBundle:Welcome:index.html.twig`` şeklindeki mantıksal
isimleri dosya yolu olarak ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig`` 
şekline çevrilir.


Templates become even more interesting when you realize they don't need to be
stored on the filesystem. You can easily store them in a database table for
instance.

Bundle'ların Genişletilmesi
............................
Eğer :doc:`bundle inheritance</cookbook/bundles/inheritance>` dokümanındaki 
kuralları uygularsanız bundle'ların dosyalarına, controllerlarına ya da şablonlarına
ulaşabilirsiniz.
Örneğin - ``AcmeNewBundle`` - adında bir bundle yarattınız ve bunun akrabası 
(parent) olarak da ``AcmeDemoBundle`` yaptınız. Symfony2 
``AcmeDemoBundle:Welcome:index``controllerinı yüklerken, ilk önce  
``AcmeNewBundle`` içerisindeki ``WelcomeController`` sınıfına bakacak ve sonra
``AcmeDemoBundle`` içine bakacaktır. Bunun anlamı bir bundle diğer bir bundle'ın
bir kısmına erişebilir!

Şimdi Symfony2 neden çok esnek anladınız mı ?. Uygulamalarınız arasındaki
bundle'ları paylaşın, onları yerel ya da global olarak saklayın. Sizin seçiminiz.

.. _using-vendors:

Vendor'ları kullanmak
----------------------
Belirli bir oranda uygulamalarının 3. parti kütüphanelere ihtiyaç duyar.
Bunlar ``vendor/`` dizini altında bulunmalıdır.Bu dizin aynı zamanda Symfony2
kütüphanelerini, SwiftMailer kütüphanesini,Doctrine ORM kütüphanesini,Twig
şablon kütüphanesini ve diğer 3. parti kütüphaneleri ve bundle'ları barındırır.
 
Cache ve Log'ları anlamak.
--------------------------------
Symfony2 muhtemelen etraftaki en hızlı full-stack framework'dür. Peki nasıl
her istek için bir sürü YAML ve XML dosyasını o koyup yorumlaması 
gerekirken bu kadar hızlı olabiliyor. Bu hız bir parça cache (önbellek) 
sistemine bağlıdır. Uygulama,konfigürasyonu sadece en önemli istek için
yorumlanır ve PHP koduna çevrilerek ``app/cache/`` dizininde saklanır.
Geliştirme ortamında Symfony2 her değişiklik için cache'i yeniden oluşturur.
Ancak bitmiş ürün  (Production) ortamında kod ya da konfigüasyon 
değişikliğinde cache'lerin boşaltılması sizin sorumluluğunuz altındadır.


Uygulama geliştirme esnasında pek çok şey pek şok sebepten yanlış gidebilir.
``app/logs/`` dizininde saklanan log dosyaları isteklerin durumu hakkında
size herşeyi sunarak problemi çabucak düzelmenizi sağlar.

Komut Satırı Arabirimini Kullanmak
-----------------------------------

Her uygulama (``app/console``) altında verilen uygulamanıza bakım uygulamak
için kullanabileceğiniz bir komut satırı yorumlayıcısı ile birlikte gelir.
Bu komutlar üretkenliğinizin arttırılmasına fayda sağlayarak sürekli tekrarlanan
süreçleri otomatikleştirmenize yardımcı olur.

Komut satırının yapabileceklerini görmek için komut satırını herhangibir
argüman olmadan aşağıdaki gibi çalıştırın.

.. code-block:: bash

    php app/console

``--help`` seçeneği size bir komut hakkında daha detaylı bilgiler verecektir.

.. code-block:: bash

    php app/console router:debug --help

Son Sözler
--------------

Beni deli olarak nitelendirebilirsiniz ancak bu bölümü okuduktan sonra 
sizin rahatınız ve konforunuz için etrafta sizn için Symfony2 ile şeyler 
yaptığımızı anlamışsınızdır. Symfony2'de yapılan herşey sizi yoldan çıkartmak
içindir. Yani istediğiniz gibi klasörlerin adlarını değiştirebilir ve kafanıza
göre bir şeyler yapabilirsiniz.

Bu hızlı turun sonuna geldik. Symfony2 ustası olabilmek için testlerden 
e-posta göndermeye kadar pek çok konuda bilgi sahibi olmak gerekir. 
Bu konuları incelemeye hazırmısınız. Daha fazla dayanamıyorsanız resmi
:doc:`/book/index` dokümana gidip bir konu başlığı seçin.

.. _standartları:  http://symfony.com/PSR0
.. _adlandırma: http://pear.php.net/
