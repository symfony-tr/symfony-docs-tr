.. index::
   single: Kurulum

Symfony'yi Kurmak ve Konfigüre Etmek
=====================================

Bu bölümün amacı Symfony üzerinde çalışan bir uygulamayı kurup çalıştırmaktır.
Çok şükür, Symfony *dağıtımı* hemen kurup çalıştırabilmeniz için fonksiyonel 
bir *başlangıç* projesi ile gelir. 

.. tip::

    Eğer kaynak kontrorlü ile birlikte en iyi nasıl proje yaratacağınız 
    konusunaki talimatları arıyorsanız `Kaynak Kontrolü'nün Kullanımı`_ 'na
    bakın.

Bir Symfony2 Dağıtımını İndirmek
-----------------------------------

.. tip::

    Öncelikle web sunucu (Apache gibi) yazılımınızın PHP 5.3.2 ve üstü 
    bir sürüm ile çalışabilecek şekilde ayarlandığınan emin olun. 
    Symfony2'nin gereksinimler için :doc:`gereksinimler belgesine </reference/requirements>`.
    bakın.
    Web sunucu dosyalarının ana yerini özelleştirmek için  `Apache`_ | `Nginx`_ 
    dökümanlarına bakın.


Symfony2 "dağıtımı", Symony2'nin çekirdek kütüphaneleri, kullanışlı, seçilmiş bundle'lar , 
tutarlı bir dizin yapısı ve bazı varsayılan konfigürasyonlarla birlikte paketlenmiş halidir.
Bir Symfony2 dağıtımını indirdiğinizde hemen uygulama geliştirilebilecek şekilde düzenlenmiş
fonksiyonel bir uygulama iskeletini indirmiş olursunuz.

`http://symfony.com/download`_ adresinde bulunan Symfony2 indirme sayfası
ile başlayın. Bu sayfada ana Symfony2 dağıtımı olarak *Symony2 Standart Edition* 'u
göreceksiniz. Burada iki seçenekten birisini seçmeniz gerekecek.

* ``.tgz`` y da  ``.zip`` olarak indirebilirsiniz. - İkiside aynıdır, size
  hangisi en uygun geliyorsa onu indirebilirsiniz;

* Dağıtımı vendor(sağlayıcı) 'lar olmadan indirin. Eğer bilgisayarınızda 
  `Git`_  kuruluysa, Symfony2 vendor olmadan (without vendors) seçeneğini
  kullanabilirsiniz. Bu size 3. parti / sağlayıcılar konusunda daha esneklik
  sağlar.
  
Bu paketlerden birisini indirdiğinizde, web sunucunuzun document root 
kısmına açmalısınız. UNIX komut satırından bu işlemi şu şekilde yapabilirsimiz.
(``###`` yerine dosya isminizi yazmalısınız)

.. code-block:: bash

    # .tgz dosyası için
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # .zip dosyası için
    unzip Symfony_Standard_Vendors_2.0.###.zip

Bu işlemi tamamladığınızda ``Symfony/`` adında ve şu şekilde gözüken 
bir klasörünüz olmalı:

.. code-block:: text

    www/ <-  web root klasörünüz
        Symfony/ <- açılmış arşiv
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

Vendorları Güncellemek
~~~~~~~~~~~~~~~~~~~~~~~

Sonunda eğer "without vendors" seçeneğindeki arşivi indirdiyseniz, Vendor
ları komut satırından aşağıdaki komutu kullanarak çalıştırmalısınız:

.. code-block:: bash

    php bin/vendors install

Bu komut tüm gerekli olan vendor kütüphanelerini - Symfony'i de kapsayan
şekilde -  ``vendor/`` klasörüne indirecektir. 3. parti vendorlar hakkında
bilgi almak ve Symfony2 içerisinde nasıl yönetilecğeini öğrenmek için 
":ref:`cookbook-managing-vendor-libraries`" belgesini okuyun.


Konfigürasyon ve Kurulum
~~~~~~~~~~~~~~~~~~~~~~~
Bukontada gerekli tüm 3. parti kütüphaneler ``vendor/`` klasörü içerisinde
bulunmaktalar. Ayrıca varsayılan uygulama ayarları ``app/`` klasörü içerisinde
ve bazı örnek kodlar ``src/`` klasörünün içerisindedir.

Symfony2, websunucunuzun ve PHP'nin Symfony ile çalışabilmesini test 
edebilmek için görsel bir sunucu konfigürasyon test aracı ile birlikte gelir.
Şu URL'yi kullanarak konfigürasyonunuzu kontrol edebilirsiniz:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Eğer bazı sorunlar varsa onları düzeltip yeniden bunu çalıştırın.

.. sidebar:: Erişim haklarını ayarlamak

    Genel bir kural olarak ``app/cache`` ve ``app/logs`` klasörüleri web
    sunucu ve komut satırı kullanıcısı tarafından yazılabilir olmalıdır.
    UNIX sistemlerinde eğer web sunucu kullanıcısı komut satırı kullanıcısından
    farklı ise, projeye başlamadan hemen önce şu komutları çalıştırarak 
    doğru erişim ayarlamalarını sağlayabilirsiniz.
    
    Web sunucunuzun ``www-data`` kullanıcısını değiştirin:

    **1. chmod +a destekleyen ACL sistemini kullanmak**
	
	Pek çok sistem ``chmod +a`` komutunu destekler. Öncelikle bunu deneyin
	eğer bir hata mesajı alırsanız diğer metodu deneyin:
	
    .. code-block:: bash

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. chmod +a desteklemeyen ACL sistemini kullanmak**
	
	Bazı sistemler ``chmod +a`` 'yı desteklemek yerine ``setfacl`` yardımcı
	aracını destekler. Partisyonunuza `ACL desteği vermek`_ için öncelikle 
	``setfacl`` uygulamasını şu şekilde (örneğin Ubuntuda) yüklemelisiniz:

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs

    Note that not all web servers run as the user ``www-data``. You have to
    check which user the web server is being run as and put it in for ``www-data``.
    This can be done by checking your process list to see which user is running
    your web server processes.

    **3. ACL Kullanmadan**
	
	Eğer ACL ile dizinleri değiştirmeye yetkiniz yoksa cache ve log dizinlerinin
	umask larının gurup yazılabilir yada herkez yazılabilir (Eğer web sunucu
	kullanıcısının komut satırı kullanıcısı ile aynı gurupta olup olmamasına
	bağlı olarak) değişitirlmesi gereklidir.
	Bunun gerçekleştirilmesi için ``app/console``, ``web/app.php`` ve 
	``web/app_dev.php``dosyalarının başına şu satırları eklemelisiniz:

    .. code-block:: php

        umask(0002); // Bu erişimi 0775 yapacaktır.

        // ya da

        umask(0000); // Bu erişimi 0777 yapacaktır.

    Unutmayın. Eğer ACL erişiminiz varsa bunu kullanmanızı tavsiye edilir.
    Çünkü web sunucusu üzerinde umask değişimi o kadar süreç-güvenli değildir.


Herşey yolundaysa, "Go to the Welcome page" 'e tıklayarak ilk "gerçek" 
Symfony2 web sayfasına bir istekte bulunun:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 size hoşgeldiniz demiş ve bu sıkı çalışmanızdan dolayı sizi
kutlamış olmalı!

.. image:: /images/quick_tour/welcome.jpg

Geliştirmeye Başlamak
---------------------
Şimdi geliştirmeye başlamak için tamamen çalışan bir Symfony2 uygulamasına
sahipsiniz! . Dağıtımınız bazı örnek kodlar içermektedir. Dağıtımınız 
içerisinde bulunan ``README.rst`` dosyasını (bu düz metin dosyasıdır) 
açarak örnek kodun dağıtımınız içerisinde neler içerdiğini ve bunu daha 
sonra nasıl silebileceğinizi öğrenebilirsiniz.

Eğer Symfony'de yeni iseniz, sayfaları nasıl yaratacağınızı,
konfigürasyonun nasıl değiştireceğinizi ve yeni uygulamanızda
ihtiyacınız olan herşeyi nasıl yapacağınızı öğrenmek için ":doc:`page_creation` 'a 
katılın.

Kaynak Kontrolü'nün Kullanımı
-----------------------------
Eğer ``Git`` ya da ``Subversion`` gibi bir sürüm kontrol sistemi kullanıyorsanız,
sürüm kontrol sisteminizi kurabilir ve projelerinizi normalde nasıl yapıyorsanız
commit edebilirsiniz. Symfony2 Standart sürümü yeni projeniz için bir başlangıç
noktasıdır.

Git üzerinde en iyi şakilde projenizi nasıl ayarlayıp saklayabileceğiniz
hakkındaki talimatlar için :doc:`/cookbook/workflow/new_project_git` belgesine
bakın.

``vendor/`` Klasörünü Yok Saymak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Eğer indirdiğiniz arşiv dosyası *without vendors* ise güvenli bir şekilde
``vendor/`` klasörünü silebilir ve kaynak kontrolüne dahil etmeyebilirsiniz.
``Git`` ile bunu yapmak için ``.gitignore`` dosyasına şunu eklemeniz yeterlidir:

.. code-block:: text

    vendor/

Şimdi vendor klasörü kaynak kontrolünde commit edilmeyecek. Bu güzel (aslında harika!).
Çünkü ne zaman birisi projenizi kopyalasa ya da projenize check out yapsa basitçe
``php bin/vendors install`` betiğini çalıştırarak kendisine gerekli vendor'ları
alabilir.

.. _`ACL desteği vermek`: https://help.ubuntu.com/community/FilePermissionsACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
