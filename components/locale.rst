.. index::
   single: Locale

Locale Bileşeni
===============

    Locale bileşeni ``intl`` eklentisi olmadığı durumlarda bu özelliğin yerine geçecek kodu sağlar.
    Buna ek olarak doğal :phpclass:`Locale` php sınıfını genişleterek bazı kullanışlı metodlarda sağlar.

Aşağıdaki intl fonksiyonlarını yerine kullanılabilecek fonksiyonlar ve 
sınıflar sıralanmıştır:

* :phpfunction:`intl_is_failure`
* :phpfunction:`intl_get_error_code`
* :phpfunction:`intl_get_error_message`
* :phpclass:`Collator`
* :phpclass:`IntlDateFormatter`
* :phpclass:`Locale`
* :phpclass:`NumberFormatter`

.. note::

     Stub implementation'u sadece ``en`` yerelinde(locale) sağlanır.
     

Kurulum
------------

Bu bileşeni üç farklı şekilde kurabilirsiniz:

* Resmi Git deposundan (https://github.com/symfony/Locale);
* PEAR ile ( `pear.symfony.com/Locale`);
* Composer ile (Packagist 'deki`symfony/locale`).

Kullanımı
---------

ClassLoader bileşeninin kullanımında aşağıdaki kodu ``intl`` eklentisi
olmadığı zamanlarda kullanımına dikkat edin:


.. code-block:: php

    if (!function_exists('intl_get_error_code')) {
        require __DIR__.'/path/to/src/Symfony/Component/Locale/Resources/stubs/functions.php';

        $loader->registerPrefixFallbacks(array(__DIR__.'/path/to/src/Symfony/Component/Locale/Resources/stubs'));
    }

:class:`Symfony\\Component\\Locale\\Locale` class enriches native :phpclass:`Locale` class with additional features:

.. code-block:: php

    use Symfony\Component\Locale\Locale;

    // Yerel için Ülke isimleri ya da tüm ülke kodlarını getir
    $countries = Locale::getDisplayCountries('pl');
    $countryCodes = Locale::getCountries();

    // Yerel için Lisan isimleri ya da tüm Lisan kodlarını getir
    $languages = Locale::getDisplayLanguages('fr');
    $languageCodes = Locale::getLanguages();

    // Verilen kod için yerel ismini ya da tüm yerel kodlarını getir
    $locales = Locale::getDisplayLocales('en');
    $localeCodes = Locale::getLocales();

    // ICU sürümlerini getir
    $icuVersion = Locale::getIcuVersion();
    $icuDataVersion = Locale::getIcuDataVersion();

