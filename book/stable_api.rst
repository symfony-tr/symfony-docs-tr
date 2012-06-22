.. index::
   single: Stable API

Symfony2 Kararlı API
=====================

Symfony2 Kararlı API tüm Symfony2 nin yayınlanan genel metodlarının
(bileşenler ve çekirdek bundle'lar) aşağıdaki özelliklerine göre yayınlanmış
halidir:

* namespace ve sınıf isimleri değişmeyecekler;
* method adı değişmeyecekler;
* method imzaları (argumanlar ve döndürülen değer tipleri) değişmeyecekler;
* metodun yaptığını şeyin kavramsal olarak değişmeyeceği;

Uygulamanın kendisini değiştirilebilir. Kararlı API içerisinde değiştirilmesine
neden olacak olan tek şey güvenlik ile ilgili olan durumlara bağlı yapılacak
değişikliklerdir.

Kararlı API `@api` olarak etiketlenen bir beyaz listeden türer. Bu yüzden
herşey Karalı API'nin bir parçası olmadan etiketlenmez.

.. tip::

    Herhangi 3. parti bundle kendisine ait Kararlı bir API ile yayınlanabilmelidir.

Symfony 2.0 gibi aşağıdaki bileşenlerinde kendilerinde genel etiketli 
API'leri mevcuttur:

* BrowserKit
* ClassLoader
* Console
* CssSelector
* DependencyInjection
* DomCrawler
* EventDispatcher
* Finder
* HttpFoundation
* HttpKernel
* Locale
* Process
* Routing
* Templating
* Translation
* Validator
* Yaml
