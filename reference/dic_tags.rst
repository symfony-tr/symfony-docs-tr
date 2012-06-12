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
| `templating.helper`_              | Servisi PHP şablonları için kullanılabilir yap                            |
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

**Amaç**: To use a custom logging channel with Monolog

Monolog allows you to share its handlers between several logging channels.
The logger service uses the channel ``app`` but you can change the
channel when injecting the logger in a service.

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

    This works only when the logger service is a constructor argument,
    not when it is injected through a setter.

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**Amaç**: Add a custom processor for logging

Monolog allows you to add processors in the logger or in the handlers to add
extra data in the records. A processor receives the record as an argument and
must return it after adding some extra data in the ``extra`` attribute of
the record.

Let's see how you can use the built-in ``IntrospectionProcessor`` to add
the file, the line, the class and the method where the logger was triggered.

You can add a processor globally:

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

You can add also a processor for a specific handler by using the ``handler``
attribute:

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

You can also add a processor for a specific logging channel by using the ``channel``
attribute. This will register the processor only for the ``security`` logging
channel used in the Security component:

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

    You cannot use both the ``handler`` and ``channel`` attributes for the
    same tag as handlers are shared between all channels.

routing.loader
--------------

**Amaç**: Register a custom service that loads routes

To enable a custom routing loader, add it as a regular service in one
of your configuration, and tag it with ``routing.loader``:

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

**Amaç**: Necessary when creating a custom authentication system

This tag is used when creating your own custom authentication system. For
details, see :doc:`/cookbook/security/custom_authentication_provider`.

security.remember_me_aware
--------------------------

**Amaç**: To allow remember me authentication

This tag is used internally to allow remember-me authentication to work. If
you have a custom authentication method where a user can be remember-me authenticated,
then you may need to use this tag.

If your custom authentication factory extends
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
and your custom authentication listener extends
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
then your custom authentication listener will automatically have this tagged
applied and it will function automatically.

security.voter
--------------

**Amaç**: To add a custom voter to Symfony's authorization logic

When you call ``isGranted`` on Symfony's security context, a system of "voters"
is used behind the scenes to determine if the user should have access. The
``security.voter`` tag allows you to add your own custom voter to that system.

For more information, read the cookbook article: :doc:`/cookbook/security/voters`.

swiftmailer.plugin
------------------

**Amaç**: Register a custom SwiftMailer Plugin

If you're using a custom SwiftMailer plugin (or want to create one), you can
register it with SwiftMailer by creating a service for your plugin and tagging
it with ``swiftmailer.plugin`` (it has no options).

A SwiftMailer plugin must implement the ``Swift_Events_EventListener`` interface.
For more information on plugins, see `SwiftMailer's Plugin Documentation`_.

Several SwiftMailer plugins are core to Symfony and can be activated via
different configuration. For details, see :doc:`/reference/configuration/swiftmailer`.

templating.helper
-----------------

**Amaç**: Make your service available in PHP templates

To enable a custom template helper, add it as a regular service in one
of your configuration, tag it with ``templating.helper`` and define an
``alias`` attribute (the helper will be accessible via this alias in the
templates):

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

**Amaç**: To register a custom service that loads translations

By default, translations are loaded form the filesystem in a variety of different
formats (YAML, XLIFF, PHP, etc). If you need to load translations from some
other source, first create a class that implements the
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` interface::

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

Your custom loader's ``load`` method is responsible for returning a
:Class:`Symfony\\Component\\Translation\\MessageCatalogue`.

Now, register your loader as a service and tag it with ``translation.loader``:

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

The ``alias`` option is required and very important: it defines the file
"suffix" that will be used for the resource files that use this loader. For
example, suppose you have some custom ``bin`` format that you need to load.
If you have a ``bin`` file that contains French translations for the ``messages``
domain, then you might have a file ``app/Resources/translations/messages.fr.bin``.

When Symfony tries to load the ``bin`` file, it passes the path to your custom
loader as the ``$resource`` argument. You can then perform any logic you need
on that file in order to load your translations.

If you're loading translations from a database, you'll still need a resource
file, but it might either be blank or contain a little bit of information
about loading those resources from the database. The file is key to trigger
the ``load`` method on your custom loader.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**Amaç**: To register a custom Twig Extension

To enable a Twig extension, add it as a regular service in one of your
configuration, and tag it with ``twig.extension``:

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

For information on how to create the actual Twig Extension class, see
`Twig's documentation`_ on the topic or read the cookbook article:
:doc:`/cookbook/templating/twig_extension`

Before writing your own extensions, have a look at the
`Twig official extension repository`_ which already includes several
useful extensions. For example ``Intl`` and its ``localizeddate`` filter
that formats a date according to user's locale. These official Twig extensions
also have to be added as regular services:

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

**Amaç**: Create your own custom validation constraint

This tag allows you to create and register your own custom validation constraint.
For more information, read the cookbook article: :doc:`/cookbook/validation/custom_constraint`.

validator.initializer
---------------------

**Amaç**: Register a service that initializes objects before validation

This tag provides a very uncommon piece of functionality that allows you
to perform some sort of action on an object right before it's validated.
For example, it's used by Doctrine to query for all of the lazily-loaded
data on an object before it's validated. Without this, some data on a Doctrine
entity would appear to be "missing" when validated, even though this is not
really the case.

If you do need to use this tag, just make a new class that implements the
:class:`Symfony\\Component\\Validator\\ObjectInitializerInterface` interface.
Then, tag it with the ``validator.initializer`` tag (it has no options).

For an example, see the ``EntityInitializer`` class inside the Doctrine Bridge.

.. _`Twig's documentation`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`Twig official extension repository`: http://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.0/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`SwiftMailer's Plugin Documentation`: http://swiftmailer.org/docs/plugins.html
