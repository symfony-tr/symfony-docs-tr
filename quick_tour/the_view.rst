Görünüm (view)
========
Bu öğreticinin ilk kısmını okuduktan sonra Symfony2 için bir 10 dakika
daha ayırmaya karar verdiniz. Mükemmel bir seçim!. Bu ikinci kısımda, 
Symfony2 şablon motoru `Twig`_. hakkında daha fazla bilgi öğreneceksiniz.
Twig PHP için, esnek, hızlı ve güvenli bir şablon motorudur. Şablonları
daha kolay okunabilir ve anlaşılabilir yapar. Aynı zamanda web tasarımcıları 
için de kullanıcı dostu hale getirir.

.. note::

    Twig yerine aynı zamanda :doc:`PHP </cookbook/templating/PHP>`
    de şablon olarak kullanabilirsiniz. İki şablon motoru da Symfony2
    tarafından desteklenebilir.

Twig ile samimi olmak
--------------------------

.. tip::

    Eğer Twig öğrenmek istiyorsanız resmi `belgesini`_ mutlaka okumanızı 
    tavsiye ediyoruz. Bu kısım sadece ana temeller üzerine
    yoğunlaşacaktır.

Bir Twig şablonu herhangi bir tipte (HTML,
XML, CSV, LaTeX, ...) içerik yaratan bir metin dosyasıdır.
Twig iki tip ayıcı belirler:

* ``{{ ... }}``: Bir değişken ya da bir ifadenin dönüşünü Ekrana Yazar;

* ``{% ... %}``: Şablonun  örneğin, ``for`` döngüleri ``if`` koşulu gibi mantıksal 
kontrollerini çalıştırır.

Aşağıda çok basit bir şablonda ``page_title`` ve  ``navigation``
adındaki tanımlanan değişkenlerin şablona aktarılması gösterilmiştir.

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>My Webpage</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>


.. tip::

   Şablonlar içerisinde kullanılan yorum satırları ``{# ... #}`` ayraçları 
   içerisinde belirtilirler.

Symfony'de bir şablonu ekrana basmak ve şablona içerisine aktarılacak değişkenler
için Controller içerisinde ``render``metodu kullanılır.

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

Şablon içerisine aktarılacak değişenkenler string, array ya da nesne tipinde
olabilir. Twig özet olara gir değişkenin içerisindeki bir niteliğe erişmek için
(``.``)  nokta işareti kullanılır.

.. code-block:: jinja

    {# array('name' => 'Fabien') #}
    {{ name }}

    {# array('user' => array('name' => 'Fabien')) #}
    {{ user.name }}

    {# force array lookup #}
    {{ user['name'] }}

    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# force method name lookup #}
    {{ user.name() }}
    {{ user.getName() }}

    {# pass arguments to a method #}
    {{ user.date('Y-m-d') }}

.. note::

    Burada dikkat edilmesi gereken şey küme parantezlerinin değişkene 
    ait olmadığı sadece şablon içerisinde değişken değerini ekrana yazdımak
    amacıyla kullanıldığınır. Eğer etiketler içerisinde bir değişkenin değerine
    ulaşmak için küme parantezi kullanmayın.
    

Şablonları Süslemek
--------------------
Çoğunlukla şablonlar proje içerisinde en genel bilinen şekliyle başlık (header) ve
sayfa sonları (footer) olarak paylaşılan öğelerdir.
Symfony2'de böyle bir sorunu farklı şekilde düşünürüz: bir şablon başka bir
şablon tarafından dekore edilebilir. Bu aslında aynı PHP sınıfları gibidir: şablonların
birbileri arasında miras alınmasına olanak verlmesi size temel "plan(layout)" adıyla
şablon içerisindeki siteniz için gereken temel "bloklar" gibi şeyler diğer şablonlar
tarafından kullanılmasına / değiştirilmesine olanak sağlar.

``hello.html.twig`` şablonu ``layout.html.twig``şablonundan ``extends`` 
etiketi aracılığı ile miras alır:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

``AcmeDemoBundle::layout.html.twig`` yazımı şablon adına çok benziyor değilmi ?

Bu referans verilen gerçek şablon yazımı ile aynıdır . ``::``  kısmı basitçe
controller elementinin boş olduğunu ifade eder ve bu yüzden ilgili dosya 
direkt ``Resources/views/`` klasörü altında tutulur.

Şimdi basit bir ``layout.html.twig`` dosyasını inceleyelim:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

``{% block %}`` etikti içerisinde tanımlanan bloklar child şablonlar 
tarafından doldurulabilir.

Tüm block etiketleri şablon motoruna child şablonun şablonun bu kısmını
değiştirebileceğini söyler.

Bu örnekte ``hello.html.twig`` şablonu ``content`` bloğunu düzenlemektedir.
Bunun anlamı "Hello Fabien"  metni ``div.symfony-content`` elementi içerisinde
çıkmaktadır.

Etiketleri, Filitreleri ve Fonksiyonları Kullanmak
--------------------------------------------------

Twig'in en önemli özelliği etiketler filitreler ve fonksiyonlar yardımıyla 
genişletilebilmesidir. Symfony2 şablon tasarımında bunun için önceden 
tanımlanmış pek çok özellikle birlikte gelir.

Diğer şablonlardan Aktarmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Paylaşılan bir kodu farklı şablonlar arasında paylaştırmanın en kolay yolu,
diğer şablonlarında ulaşabileceği bir yeni şablon yaratmaktır.

``embedded.html.twig`` şablonunu şu şekilde yaratalım:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

Ve ``index.html.twig`` şablonunu bunu içerecek şekilde düzenleyelim:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# body bloğu embedded.html.twig tarafından düzenleniyor...#}
    {% block content %}
        {% include "AcmeDemoBundle:Demo:embedded.html.twig" %}
    {% endblock %}

Diğer Controller'ları İçeri Gömmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Başka bir controller'in sonucunu şablonunuza gömmek isterseniz? 
Bu özellikle Ajax üzerinde çalışırken ya da ana şablonda bulunmayan
bazı değişkenleri şablon içerisinde kullanmak istediğinizde çok 
kullanışlıdır.

Varsayılımki siz bir ``örnek`` action yaratınız ve bunu ``index`` şablonu
içerisine aktarmak istiyorsunuz. Bunu yapmak için ``render`` etiketi ile yaparız:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {% render "AcmeDemoBundle:Demo:ornek" with { 'name': name, 'color': 'green' } %}

Burada ``AcmeDemoBundle:Demo:ornek`` metni ``Demo`` controller içerisindeki
``ornek`` actionunu ifade eder. Argümanlar (``name`` ve ``color``) istek
değişkenlerini kontrol ederler (eğer orneKAction bu argümanlarla yeni bir istek
alırsa)::


    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // $color değişkenine bağlı bir nesne yarat
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array('name' => $name, 'object' => $object));
        }

        // ...
    }

Sayfalar arasında Link Vermek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web uygulamalarının konuşması için sayfalar arasında linkler oluşturuması
gereklidir. Şablolnlarda karmış URL adresleri kullanmak yerine ``path`` 
fonksiyonu route konfigürasyonundan referans alarak bu URL adreslerini yaratır.
Bu yol, tüm URL'lerinizi konfigürasyonda değiştirilerek basitçe değiştirilebilir:

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>


``path`` fonksiyonu route adını ve argümanları içeren dize değişkenini alır.
Route adı hangi routeların ve yer tutucular içerisinde değer içeren parametrelerin
ve route deseni içerisinde yapılan tanımlamaların ana anahtarıdır::


    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    ``url`` fonksiyonu  *mutlak* URL üretir: ``{{ url('_demo_hello', {
    'name': 'Thomas' }) }}``.

Varlıkları dahil etmek: resimler, JavaScriptler, and stil şablonları
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Resimler, JavaScriptler v stil şablonları olmadan internet nasıl olurdu ?
Symfony2 ``asset`` fonksiyonu ile bunlarla kolaylıkla çalışabilir:

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

``asset`` fonksiyonunun ana amacı uygulamanızı daha taşınabilir yapmaktıd.r
Bu fonksiyon sayesinde uygulamanızın kök diznini web kök dizininiz içerisinde
şablon kodunda herhangi bir değişiklik yapmadan kolaylıkla istediğiniz yere
taşıyabilirsiniz.

Değişken kaçışları (Escaping Variables)
----------------------------------------

Twig tüm çıktıları otomatik olarak kaçışlar. Twig `belgesini`_ okuyarak 
Escaper eklentisinin nasıl çalıştığı hakkında daha fazla bilgiye sahip 
olabilirsiniz.

Son Sözler
--------------
Twig gerçekten güçlüdür. Yerleşim planları, bloklar, şablonlar ve işlem
fonksiyonları sayesinde şablonunuzu çok kolay bir şekilde organize edebilir
ve genişletebilirsiniz. Eğer kendinizi Twig ile rahat hissetmiyorsanız
dilediğiniz her zaman PHP  şablonlarını Symfony içerisinde bir şarta
bağlı olmadan kolaylıkla kullanabilirsiniz.

Sadece 20 dakikadan beri Symfony2 ile çalışıyorsunuz ancak daha şimdiden
odukça fazla inanılmaz şey başardınız. Bu Symfony2'nin gücüdür.
Temlleri öğrenmek kolaydır ve biraz sonra öğreneceğiniz gibi bu basitliğin
altında oldukça esnek bir mimari bulunmaktadır.

But I'm getting ahead of myself. Öncelikle controller hakkında daha fazla 
şey bilmelisiniz bunun için mutlaka :doc:`bu öğreticinin sonraki bölümünü<the_controller>`
okumalısınız.
Symfony2 ile başka bir 10 dakikaya hazır mısınız ?

.. _Twig:          http://twig.sensiolabs.org/
.. _belgesini: http://twig.sensiolabs.org/documentation
