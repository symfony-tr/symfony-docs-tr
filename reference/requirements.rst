.. index::
   single: Gereksinimler
   
Symfony2'nin Çalışması için Gereksinimler
=========================================

Symfony2'nin çalışabilmesi için yapmanız gereken bazı ayarlar ve yüklemeler bulunmaktadır. 
Kolaylıkla görebileceğiniz gibi sistemin ihtiyacı olan tüm gereksinimler ``web/config.php``
betiği ile Symfony dağıtımında saptanabilir.PHP CLI sıklıkla farklı ``php.ini`` dosyası kullanmasından dolayı
komut satırınında gereksinimlerini şu şekilde belirlenebilir:

.. code-block:: bash

    php app/check.php

Aşağıda gerekli ve opsiyonel olarak gereken ihtiyaçlar sıralanmıştır.

Gerekli
--------

* PHP en az PHP 5.3.2 sürümünde olmalıdır.
* Sqlite3 aktif edilmelidir.
* JSON aktif edilmelidir.
* ctype aktif edilmelidir.
* PHP.ini dosyanızda date.timezone ayarı yapılmış olmalıdır.

Opsiyonel
--------

* PHP-XML modülüne ihtiyacınız olabilir.
* En az libxml 2.6.21 sürümünde olmalıdır.
* PHP tokenizer açık olmalıdır.
* mbstring fonksiyonları açık olmalıdır.
* iconv açık olmalıdır.
* POSIX açık olmalıdır (sadece \*nix lerde)
* Intl ,ICU 4+ ile birlikte kurulmalıdır.
* APC 3.0.17 ve üzeri (yada başka bir önbellekleme sistemi)
* PHP.ini tavsiye edilen ayarları

  * ``short_open_tag = Off``
  * ``magic_quotes_gpc = Off``
  * ``register_globals = Off``
  * ``session.autostart = Off``

Doctrine
--------

Eğer Doctrine kullanmak isterseniz PDO kurulu olmalıdır.Ek olarak, PDO
sürücüsü de kullanmak istediğiniz veritabanı sunucusu üzerinde de kurulu olmalıdır.
