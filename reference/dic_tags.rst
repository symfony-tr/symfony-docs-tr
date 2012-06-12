Dependency Injection Etiketleri
===============================

Dependency Injection Etiketleri bir servise öze olarak bir "işaret"(flag) 
koymak için kullanılan kısa metinlerdir. Mesela eğer Symfony'nin çekirdek olayları 
(core events) tarafından dinlenmesi gereken bir servisiniz var ise bunu
``kernel.event_listener`` etiketi altında işaretleyebilirsiniz.

Aşağıda Symfony2 içerisinde mevcut olan tüm etiketler listelenmiştir:

+-----------------------------------+---------------------------------------------------------------------------+
| Etiket Adı                        | Kullanımı                                                                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `data_collector`_                 | Profiler için özel veriler toplayan bir sınıf yarat.                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type`_                      | Özel bir form alan tipi yarat                                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_extension`_            | Özel "form eklentisi" yarat                                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_guesser`_              | "form tipi tahmini" için kendi algoritmanızı ekler                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_warmer`_            | Servisinizi cache warming süreci çalıştırıması esnasında çağırır          |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_listener`_          | Symfony'de farklı event'lar/hook'lar dinlemek                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.logger`_                 | Farklı bir loglama kanalından log kayıtları yap                           |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.processor`_              | Loglama için farklı bir işlemci (processor) ekle                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `routing.loader`_                 | Route'ları yükleyen özel bir servisi kayıtla(register)                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.voter`_                 | Symfony'nin yetkilendirme algoritmasına özel bir karar verici(voter)ekle  |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.remember_me_aware`_     | Beni Hatırla yetkilendirmesi'ne izin ver                                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.listener.factory`_      | Özel bir yetkilendirme sistemi yaratırken gerekli                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.plugin`_             | Özel SwiftMailer Plugin'i ekle                                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | Servisinizi PHP şablonları için kullanılabilir yapın                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | Tercümeleri yükleyen özel bir servis ekle                                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.extension`_                 | Özel bir Twig Extension'u ekle                                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.constraint_validator`_ | Kendi özel veri doğrulama koşutunuzu yaratın                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.initializer`_          | Nesneleri doğrulamadan önce yüklenecek bir servisi kayıtla                |
+-----------------------------------+---------------------------------------------------------------------------+

data_collector
--------------

**Amaç**: Profiler için özel veriler toplayan bir sınıf yarat

Kendinize özel veri kolleksiyonu yaratma hakkında daha fazla bilgi için
tarif kitabındaki :doc:`/cookbook/profiler/data_collector` belgesini
okuyun.

form.type
---------

**Amaç**: Özel bir form alan tipi yarat

Kendi özel form alan tipleri yaratmak hakkında daha detaylı bilgi almak 
için tarif kitabındaki :doc:`/cookbook/form/create_custom_field_type` 
belgesini okuyun.

form.type_extension
-------------------

**Amaç**: Özel "form eklentisi" yarat

Form tip eklentileri formunuzdaki her alanın yaratımında alanların kontrolünü
almanız için bir yoldur. Örneğin CSRF token'nin bir form eklentisi tarafından
eklenmesi (:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

Bir form tipi eklentisi formunuzdaki her alanın parçasını değiştirebilir. Bir
form tipi eklentisi yaratmak için öncelikle 
:class:`Symfony\\Component\\Form\\FormTypeExtensionInterface` interface'inden
türeyen bir sınıf yaratmanız gerekir. Basitleştirmek için sıklıkla
:class:`Symfony\\Component\\Form\\AbstractTypeExtension` sınıfından form 
tipinizi türetmek yerine bu interface 'üzerinden sınfınızı direkt olarak
türeteceksiniz:: 

    // src/Acme/MainBundle/Form/Type/MyFormTypeExtension.php
    namespace Acme\MainBundle\Form\Type\MyFormTypeExtension;

    use Symfony\Component\Form\AbstractTypeExtension;

    class MyFormTypeExtension extends AbstractTypeExtension
    {
        // ... neye hükmetmek(override) istiyorsanız 
        // buildForm(), buildView(), buildViewBottomUp(), getDefaultOptions() or getAllowedOptionValues()
        // gibi metodları bu alanda kullanın
    }

Symfony nin sizin form eklentisini kullanabilmesi amacıyla tanıması için
bunu `form.type_extension` etiketi altında vermelisiniz:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.form.type.my_form_type_extension:
                class: Acme\MainBundle\Form\Type\MyFormTypeExtension
                tags:
                    - { name: form.type_extension, alias: field }

    .. code-block:: xml

        <service id="main.form.type.my_form_type_extension" class="Acme\MainBundle\Form\Type\MyFormTypeExtension">
            <tag name="form.type_extension" alias="field" />
        </service>

    .. code-block:: php

        $container
            ->register('main.form.type.my_form_type_extension', 'Acme\MainBundle\Form\Type\MyFormTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

Etiketin ``alias`` anahtarı  uygulanacak eklentinin hangi alan tipine uygulanacağını 
ifade eder. Örneğin herhangi bir alana bu eklentiyi uygulamak istiyorsanız 
"field" değerini kullanın.

form.type_guesser
-----------------

**Amaç**: "form tipi tahmini" için kendi algoritmanızı ekler

Bu etiket :ref:`Form Tahmini<book-forms-field-guessing>` işlemi için
kendi algoritmanızı kullanmanızı sağlar. Varsayılan olarak form tahmini 
ver doğrulama metadata'sı ve Doctrine metadata'larına(Eğer Doctrine kullanıyorsanız)
bağlı olan "tahminci"(guessers) 'ler tarafından yapılır.

Kendi form tahmincinizi eklemek için 
:class:`Symfony\\Component\\Form\\FormTypeGuesserInterface` interface'inden
türeyen bir ınıf yaratın. Sonra ``form.type_guesser`` (herhangi bir seçeneği olmadan)
servis tanımlaması ile etiketleyin.

Bu sınıfın nasıl görünebileceği hakkındaki örneği görmekl için ``Form``
bileşenindeki ``ValidatorTypeGuesser`` sınıfına bakın.

kernel.cache_warmer
-------------------

**Amaç**: Servisinizi cache warming süreci çalıştırıması esnasında çağırır

Cache warming işlemi ``cache:warmup`` ya da  ``cache:clear`` işlemini
çalıştırdığınızda başlar(eğer ``cache:clear`` komutuna ``--no-warmup`` 
parametresi vermezseniz). Buradaki amaç her cache başlatılmasında uygulamanın
ihtiyacı olacak ve ilk kullanıcının dinamik olarak yaratılan cache'in 
"cache hit"'inden korumak dır.

Kendi cache warmer'ınızı yaratmak için öncelikle 
:class:`Symfony\\Component\\HttpKernel\\CacheWarmer\\CacheWarmerInterface` 
interface'inden türeyen bir sınıf yaratın::

    // src/Acme/MainBundle/Cache/MyCustomWarmer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheWarmer\CacheWarmerInterface;

    class MyCustomWarmer implements CacheWarmerInterface
    {
        public function warmUp($cacheDir)
        {
            // cache'ınızı "warm" esnasnda yapacağınız işlemler
        }

        public function isOptional()
        {
            return true;
        }
    }

``isOptional`` metodu eğer uygulama bu cache warmer'la çağırılmayacaksa 
alacağı true değeri bu işlemi mümkün kılar. Symfony 2.0'da isteğe bağlı
warmer'lar herzaman herşekilde çalışır bu yüzden bu fonksityonun gerçek
bir etkisi yoktur.

Symfony'ye kendi warmer'ınızı kullandırmanız içen bunu ``kernel.cache_warmer``
etiketi altında vermelisiniz:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.warmer.my_custom_warmer:
                class: Acme\MainBundle\Cache\MyCustomWarmer
                tags:
                    - { name: kernel.cache_warmer, priority: 0 }

    .. code-block:: xml

        <service id="main.warmer.my_custom_warmer" class="Acme\MainBundle\Cache\MyCustomWarmer">
            <tag name="kernel.cache_warmer" priority="0" />
        </service>

    .. code-block:: php

        $container
            ->register('main.warmer.my_custom_warmer', 'Acme\MainBundle\Cache\MyCustomWarmer')
            ->addTag('kernel.cache_warmer', array('priority' => 0))
        ;

``priority`` değeri isteğe bağlıdır ve varsayılan değeri 0 dır. 
Bu değer -255 ile 255 arasında olabilir ve warmer'lar bu priority değerinin
büyüklüğüne göre çalıştırılacaklardır.

.. _dic-tags-kernel-event-listener:

kernel.event_listener
---------------------

**Amaç**: Symfony'de farklı event'lar/hook'lar dinlemek

Bu etiket kendi sınıfınızı Symfony'nin işlemesi sürecinde farklı noktalara 
kancalamanıza (hook) olanak verir.

Bu dinleyici (listener) için tam bir örnek istiyorsanız 
:doc:`/cookbook/service_container/event_listener` tarif kitabı girdisini
okuyun.

Kernel listener için başka pratik bir örnek için :doc:`/cookbook/request/mime_type`
tarif kitabı girdisine de bakabilirsiniz.

.. _dic_tags-monolog:

monolog.logger
--------------

**Amaç**: Monolog ile özel bir log kanaklı kullanmak
          

Monolog farklı loglama kanalları arasında işleyicileri(handler) paylaşmanıza
olanak sağlar. Loglama servisi ``app`` kanalını kullanır ancak bunu
bir loglama servisi enjekte ettiğinizde (inject) değiştirebilirsiniz.

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: [@logger]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);;

.. note::

    Bu sadece loglama servisi bir yapılandırma argümanı ise çalışır eğer
    bir setter ile enjeksiyon yapılırsa çalışmaz.

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**Amaç**: Loglama için farklı bir işlemci (processor) ekle

Monolog size loglama için yada işleyicilere kayıtlar içerisinde ekstra alan
lar eklemenize olanak sağlar. Bir işlemci(processor) bir kayıdı argüman gibi
alır ve onu kayıdın ``extra`` niteliği içeriside eklendikten sonra kayıt içerisindeki
ekstra verileri döndür.

İsterseniz loglayıcı tetiklendiğinde dosyaya satır ekleme işinin
``IntrospectionProcessor`` ile nasıl yapabileceğinizi görelim.

Global olarak bir işlemci ekleyebilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('my_service', $definition);

.. tip::

    If your service is not a callable (using ``__invoke``) you can add the
    ``method`` attribute in the tag to use a specific method.
    
    Eğer servisiniz çağıılabilir değilse (``__invoke`` kullanarak) etiket
    içerisinde özel bir metod kullanmak için ``method`` niteliğini ekleyebilirsiniz.

``handler`` niteliğini kullanarak da belirli bir işlem için özel bir 
işlemci(processor) ekleyebilirsiniz:


.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('my_service', $definition);

``channel`` niteliğini kullanarak da belirli bir loglama kanalı için bir işlemci
ekleyebilirsiniz. Aşağıdaki örnekte  sadece Security bileşeni kullanıldığında ``security`` 
kanalından loglaması için kullanılacak işlemciyi kayıt edilecektir: 

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('my_service', $definition);

.. note::

    ``handler`` ve ``channel`` niteliklerini aynı etikette paylaşılan 
    tüm kanallar arasındaki işleyiciler olarak kullanamazsınız.

routing.loader
--------------

**Amaç**: Route'ları yükleyen özel bir servisi kayıtla(register)

Özel bir route yükleyicisini çalıştırmak için konfigürasyonuzun birisinde
bunu bir servis olarak ekleyip bunu ``routing.loader`` etiketi altında 
belirtmelisini.

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

security.listener.factory
-------------------------

**Amaç**: Özel bir yetkilendirme sistemi yaratırken gerekli

Bu etiket kendi yetkilendirme (authentication) sisteminizi yaratırken 
kullanılır. Daha fazla bilgi için :doc:`/cookbook/security/custom_authentication_provider`
belgesine bakınız.

security.remember_me_aware
--------------------------

**Amaç**: Beni Hatırla yetkilendirmesi'ne izin ver

Bu etiket içsel olarak beni-hatırla (remember-me) yetkilendirmesinin
çalışmasını sağlar. Eğer bir kullanıcının beni-hatırla yetkilendirmesini
kullanan özel bir yetkilendirme mekanizması kullanıyorsanız bu etiketi kullanmanız
gerekebilir.

Eğer özel yetkilendirme factory'niz 
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
sınıfından türerse ve özel yetkilendirme dinleyicisi (listener)
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`
sınıfından türerse, özel yetkilendirme dinleyicisi otomatik olarak bu
şekilde etiketlenecek ve uygulamaya alınacaktır.

security.voter
--------------

**Amaç**: Symfony'nin yetkilendirme algoritmasına özel bir karar verici(voter)ekle

Symfony güvenlik içeriğinde, ``isGranted``  çağırıldığında "karar verici"(voter)
sisteminin arkaplanda kullanıcının yetkisinin olup olmadığı belirlenir.
``security.voter``  etiketi kendi özel "karar vericinizi" sisteme eklemenize
olanak sağlar.

Daha fazla bilgi için :doc:`/cookbook/security/voters` tarif kitabı girdisini
okuyun.

swiftmailer.plugin
------------------

**Amaç**: Özel SwiftMailer Plugin'i ekle

Eğer özel bir SwiftMailer eklentisi kullanıyorsanız(ya da bir tane yaratmak 
istiyorsanız) bunu plug'in için yaratacağınız bir servis ile ve bunu 
``swiftmailer.plugin`` (argüman olmadan) etiketi altında belirterek 
SwiftMailer ile kullanabilirsiniz.

Bir SwiftMailer eklentsi mutlaka ``Swift_Events_EventListener`` interface'i
ile yapılandırılmalıdır. Eklentiler hakkında daha fazla bilgi için 
`SwiftMailerin Eklenti Belgesi`_ 'ne bakın.

Farklı SwiftMailer eklentileri Symfony çekirdeğinde gelir. Farklı konfigürasyonlarla
bunlar aktifleştirilebilir. Daha fazla bilgi için :doc:`/reference/configuration/swiftmailer`
belgesine bakın.

templating.helper
-----------------

**Amaç**: Servisinizi PHP şablonları için kullanılabilir yapın


Özel bir şablon yardımcısını aktifleştirmek için bir konfigürasyon içerisinde
bir servis tanımlaması yaparak bunu ``templating.helper`` etiketi altında 
``alias`` niteliği ile belirtin(bu yardımcı şablonlar içerisinden alias 
tanımında  verilen değer ile erişilebilir):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

translation.loader
------------------

**Amaç**: Tercümeleri yükleyen özel bir servis ekle

Varsayılan olarak tercümeler farklı formatlarda olan (YAML, XLIFF, PHP,vs..)
dosya sistemi şeklinde yüklenir. Eğer tercümeleri başka kaynaklardan yüklemek
istiyorsanzız öncelikle 
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` interface'i
üzerinde türetilen bir sınıf yaratın::

    // src/Acme/MainBundle/Translation/MyCustomLoader.php
    namespace Acme\MainBundle\Translation;

    use Symfony\Component\Translation\Loader\LoaderInterface
    use Symfony\Component\Translation\MessageCatalogue;

    class MyCustomLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $catalogue = new MessageCatalogue($locale);

            // some how load up some translations from the "resource"
            // then set them into the catalogue
            $catalogue->set('hello.world', 'Hello World!', $domain);

            return $catalogue;
        }
    }

Özel yükleyiciniz'in ``load`` metodu :Class:`Symfony\\Component\\Translation\\MessageCatalogue`
sınıfını döndürmekten sorumludur.

Şimdi yükleyici servisini ``translation.loader`` etiketi altında kayıt edin:

.. code-block:: yaml

    services:
        main.translation.my_custom_loader:
            class: Acme\MainBundle\Translation\MyCustomLoader
            tags:
                - { name: translation.loader, alias: bin }

.. code-block:: xml

    <service id="main.translation.my_custom_loader" class="Acme\MainBundle\Translation\MyCustomLoader">
        <tag name="translation.loader" alias="bin" />
    </service>

.. code-block:: php

    $container
        ->register('main.translation.my_custom_loader', 'Acme\MainBundle\Translation\MyCustomLoader')
        ->addTag('translation.loader', array('alias' => 'bin'))
    ;

``alias`` seçeneği gerekli ve çok önemlidir. Çünki bu yükleyici (loader)
tarafından yüklenecek kaynak dosyaların "son eki"(suffix) 'ni ifade eder. 
Örneğin varsaalım özel ``bin`` tipinde tercümeler barındıran dosyaları 
yüklemeniz gerekiyor. Eğer ``messages`` alan adı için Fransızca tercümeler
olan bir dosyanız varsa bu dosyanız muhtemelen 
``app/Resources/translations/messages.fr.bin`` şeklinde olacaktır.

Symfony2 ``bin`` dosyasını yüklemeye çalışırken özel yükleyicinizin yolunu
``$resource`` argümanı olarak belirtir. Bunu tercümelerinizi yükleyen
herhangi bir algoritma altında kullanabilirsiniz.

Eğer tercümelerinizi bir veri tabanından yüklüyorsanız, bunun içinde
bir kaynak dosyasına ihtiyacınız olacak, ancak ya boş bir dosya ya da içerisinde
bu kaynağın veri tabanından yükleneceğini belirten bir bilgi olan bir
dosya olacaktır. Dosya, özel yükleyicinizin ``load`` metodundaki anahtar
şeydir.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**Amaç**: Özel bir Twig Extension'u ekle

Bir Twig eklentisini aktif hale getirmek için bunu konfigürasyondan birisi içinde
bir servis olarak tanımlayıp daha sonra ``twig.extension`` etiketi altında belirtin:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

Gerçek bir Twig Eklenti sınıfının nasıl yaratıldığı hakkında daha fazla
bilgi almak için `Twig'in kendi belgeleri`_ içerisindeki konuları okuyun
ya da tarif kitabındaki :doc:`/cookbook/templating/twig_extension` belgesini
okuyun.

Kendi eklentilerinizi yazmadan önce `Twig'in resmi eklenti deposu`_ 'na bakıp
daha önceden yapılmış faydalı bir eklenti olup olmadığına bakabilirsiniz.
Örneğin ``Intl`` ve ``localizeddate`` filitresi kullanıcıların yerel bilgisine
göre tarih verisini formatlar. Bu resmi Twig eklentileri ayrıca bir servis 
olarak tanımlanmıştır:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.intl', 'Twig_Extensions_Extension_Intl')
            ->addTag('twig.extension')
        ;

validator.constraint_validator
------------------------------

**Amaç**: Kendi özel veri doğrulama koşutunuzu yaratın

bu etiket kendi veri doğrulama koşutunuzu yaratmanıza izin verir. Daha fazla
bilgi için tarif kitabındaki :doc:`/cookbook/validation/custom_constraint`
adlı belgeye bakabilirsiniz.

validator.initializer
---------------------

**Amaç**: Nesneleri doğrulamadan önce yüklenecek bir servisi kayıtlar

Bu etiket çok nadir zamanlarda kullanılan, bir nesne doğrulanmadan(validate)
önce yapmak istediğiniz bir dizi işlemi yapmanıza olanak sağlar. Örneğin
bir nesne doğrulanmadan önce nesnedeki tüm gereken bilgilerin veritabanından
çekilmesi gibi(laizly-load). Bu olmadan Doctrine entity'sindeki bazı datalar
doğrulanma esnasında, gerçekte böyle bir durum olmamasına rağmen,eksik olabilir.

Eğer bu etiketi kullanma ihtiyacı hissediyorsanız sadece 
:class:`Symfony\\Component\\Validator\\ObjectInitializerInterface` interface'inden
türeyen bir sınıf yaatıp bunu ``validator.initializer`` etiketi (herhangi bir
argümanı olmadan) belirtmeniz gerekir.

Örnek için Doctrine Bridge içerisinde bulunan ``EntityInitializer``
sınıfına bakın.


.. _`Twig'in kendi belgeleri`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`Twig'in resmi eklenti deposu`: http://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.0/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`SwiftMailerin Eklenti Belgesi`: http://swiftmailer.org/docs/plugins.html
