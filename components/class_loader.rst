.. index::
   pair: Autoloader; Konfigürasyon

ClassLoader Bileşeni
====================

    ClassLoader bileşeni eğer bazı standart PHP kurallarını kullanıyorsa
    proje sınıflarını otomatik olarak yükler.
    
Tanımlanmamış bir sınıf kullandığınızda PHP autoloading mekanizmasını kullanarak
dosyanın içinde tanımlanmış sınıfı yüklemeye çalışır. Symfony2 sınıfları 
dosyalardan aşağıdaki kurallardan birisine göre yükleyen bir "universal" autloader
sağlar.

* PHP'nin namespace'leri ve sınıflarındaki teknik `standartlar`_ 'a göre;

* Sınıflar için `PEAR`_ isimlendirme kurallarına göre.

Eğer projenizde kullandığınız sınıflarınız ve 3.parti kütüphaneleriniz bu
standartlara uyuyorsa Symfony2 autloader'i ihtiyacınız olan tek autloader
olacaktır.

Kurulum
-------

Bu bileşeni üç farklı şekilde kurabilirsiniz:

* Resmi Git reposunu kullanarak (https://github.com/symfony/ClassLoader);
* PEAR ile yükleme ( `pear.symfony.com/ClassLoader`);
* Composer ile yükleme (Packagist 'deki `symfony/class-loader`).

Kullanımı
---------

:class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` autloader'ini
çok basit bir şekilde yükleyebilirsiniz:

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();

    // namespace 'ler ve ön ekler kayıt ediliyor- aşağıya bakın

    $loader->register();

Küçük bir performans artışı için yollar (path) APC tarafından 
:class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader` sınıfı ile
cache'lenebilir::

    require_once '/path/to/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/path/to/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

Autloader eğer bazı kütüphaneleri autload' da ekleyecekseniz kullanışlıdır.

.. note::

    autloader Symfony2 uygulamasında otomatik olarak kayıtlanır (register)
    (bkz ``app/autoload.php``).

Eğer sınıflar autload esnasında namespace'ler kullanacaksa 
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace`
metodunu ya da 
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces`
metodunu kullanın::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/src',
    ));

    $loader->register();

Sınıflarda PEAR isimlendirme kurallarını uygulayacaksanız 
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix`
metodu ya da 
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes`
metodlarını kullanın::

    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/lib',
    ));

    $loader->register();

.. note::

    Bazı kütüphaneler ayrıca kendi kök dizinlerini de PHP 
    include path (``set_include_path()``) içerisinde kayılı olmasını isterler.

Sınıflar büyük projelerde sağlanan bir sınıfın bir alt-namespace'den ya da 
bir PEAR sınıflarının bir alt-hiyerarşisinden geliyor olabilirler::


    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine-dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/lib',
    ));

    $loader->register();

Bu örnekte eğer ``Doctrine\Common`` namespace'ini ya da bunun bir altındaki 
sınıfı kullanmak isterseniz autloader ilk önce ``doctrine-common``  klasörü
altındaki sınıflara bakacak eğer önceki verilende bulunamazsa
``Doctrine`` dizinine bakacaktır (en son konfigüre edilen). Kayıtlamaların
sırası bu durumda önemlidir.

.. _standartlar: http://symfony.com/PSR0
.. _PEAR:      http://pear.php.net/manual/en/standards.php
