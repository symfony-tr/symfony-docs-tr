.. index::
   single: Şablonlar (Templating)

Şablonları Yaratmak ve Kullanmak
================================
Bildiğiniz gibi :doc:`controller </book/controller>`  Symfony2 uygulamasına
gelen her isteği işlemekten sorumludur. 
Gerçekte controller, diğer alanlardaki pek çok ağır iş için, kod test edilip
yeniden kullanılsın diye görevlendirilir.
Controller HTML,CSS ya da diğer bir içeriği yaratması gerektiğinde işi 
şablon motoruna aktarır. Bu kısımda kullanıcının içeriğini döndürmek,
e-posta gövdesini oluşturmak ve daha fazlası için güçlü şablonların nasıl 
yazılabileceğini göreceksiniz. Kısa yolları öğrenecek, şablonların zarifçe
nasıl genişletilebileceğini ve şablon kodunun nasıl yeniden kullanılabileceğini
göreceksiniz.

.. index::
   single: Templating; Şablon nedir ?

Şablonlar
---------
Bir şablon herhangi bir metin tipi ile (HTML, XML, CSV, LaTeX ...) yaratılan
basit bi metin dosyasıdır. En bilinen şablon tipi bir metin dosyasının
PHP tarafından yorumlanan, metin ve PHP kodunun karışımından oluşan *PHP* şablonudur:

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
        </head>
        <body>
            <h1><?php echo $page_title ?></h1>

            <ul id="navigation">
                <?php foreach ($navigation as $item): ?>
                    <li>
                        <a href="<?php echo $item->getHref() ?>">
                            <?php echo $item->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: Twig; Giriş

Fakat Symfony2 `Twig`_ adındaki daha güçlü bir şablonlama dili ile birlikte gelir.
Twig  size ksa, okunabilir şablonlar yazmanızı, web tasarımcıları için daha 
kolay ve çeşitli açılardan PHP şablonlarından daha güçlü bir şablon sunar.

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
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

Twig iki adet özel yazım şekline sahiptir:

* ``{{ ... }}``: "Bir Şeyler Söyler": bir değişkenin içeriğini ya da bir
  ifadenin sonucunu şablon içerisine yazar;

* ``{% ... %}``: "Bir Şeyler Yapar": şablonun mantıksal kısmını, örneğin
  for döngüleri gibi ifadeleri çalışırmak için bir **etiket**  tir.

.. note::

   Üçüncü bir ifade de yorum satırları yaratmak için kullanılan 
   ``{# Burası Yorum Satırı #}`` şeklindeki yazım türüdür.
   Bu yazım PHP'nin çoklu yorum yapılmasına olanak sağlayan 
   ``/* Yorum */`` ifadesi gibi kullanılabilir.

Twig ayrıca içeriğin ekrana basılmadan önce değiştirilmesine imkan
sağlayan **filitre** 'lere sahiptir. Aşağıdaki kod ``title`` değişkeninin
içeriğindeki tüm ifadeleri ekrana basmadan önce büyük harf haline getirir:

.. code-block:: jinja

    {{ title|upper }}

Twig varsayılan olarak `etiketler`_ ve `filitreler`_ için uzun bir liste
ile birlikte gelir. Hatta gerektiğinde Twig'e `kendi eklentinizi dahi yazabilirsiniz`_ .

.. tip::
	Bir Twig eklentisini tanıtmak yeni bir servis yaratıp onu 
	``twig.extension`` :ref:`etiketi<reference-dic-tags-twig-extension>` ile
	etiketlemek kadar kolaydır.
	
Buraya kadar sizinde görebildiğiniz gibi Twig ayrıca fonksiyonlar 
ve yeni yaratılabilecek fonksiyonları da destekler. Örneğin, aşağıdaki
örnekte standart ``for`` etiketi, on adet div etiketini ``odd`` ve
``even`` classlarını sırasıyla yazdırmak için ``cycle`` fonksiyonu 
kullanılmıştır: 

.. code-block:: html+jinja

    {% for i in 0..10 %}
        <div class="{{ cycle(['odd', 'even'], i) }}">
          <!-- some HTML here -->
        </div>
    {% endfor %}

Bu kısım boyunca şablon örnekleri Twig ve PHP ile birlikte gösterilecektir.

.. sidebar:: Neden Twig?

    Twig şablonları basit ve PHP taglarının kullanılmayacağı manasına gelmektedir.
    Bu tasarım açısından; Twig şablon sistemi hızlı sunum, program kodunun 
    olmaması anlamına gelmektedir. Daha fazla Twig kullandığınızda bu ayrımın
    faydasını daha iyi anlayacaksınız. Ve elbette her yerde web tasarımcıları
    tarafınan sevileceksiniz.
    
    Twig PHP'nin yapamadığı gerçek şablon mirası alma (Twig şablonları PHP
    sınıflarına çevrildiğinde birinden diğerine aktarılabilir), boşluk kontrolü,
    sandbox'lama  ve sadece şablonları etkileyen özelleştirilmiş fonksiyonların 
    ve filitler gibi pek çok şeyi de yapabilir. Twig şablonları daha kolay
    anlaşılır ve basit yazmk için bazı küçük özelliklere sahiptir. Aşağıdaki
    örnekte ``if`` mantıksal ifadesi ile birlikte bir döngünün kullanılması
    gösterilmektedir:
    
    .. code-block:: html+jinja
    
        <ul>
            {% for user in users %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>No users found</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Ön Bellek

Twig Şablonu Ön Bellekleme
~~~~~~~~~~~~~~~~~~~~~~~~~~
Twig hızlıdır. Her Twig şablonu çalışma esnasında doğal PHP sınıflarına çevrilir.
Derlenmiş sınıflar ``app/cache/{environment}/twig`` klasöründe 
(``{environment}`` ``dev`` ya da  ``prod`` gibi çevrelerdir) ve bazı durumlarda
hata ayıklamak için kullanılabilir. :ref:`environments-summary` belgesine
bakarak çevreler hakkında daha fazla bilgiye sahip olabilirsiniz.

``debug`` modu aktif ise (genellikle ``dev`` ortamında) bir  Twig şablonu
otomatik olarak değişiklik yapıldığında yeniden derlenecektir.Bunun anlamı
geliştirme esnasında mutlu bir şekilde Twig şablonunda değişiklikleri yaptıktan
sonra değişikliklerin etkili olması için önbellekleri temizlemenize gerek
olmadığıdır.

``debug`` modu aktif değil ise (genellikle ``prod`` ortamında) bu durumda
Twig şablonunun yeniden yaratılması için ön bellekleri boşaltmanız gereklidir.
Bunu uygulamanızı aktarma(deploy) esnasında yapacağınızı unutmayın.

.. index::
   single: Templating; Kalıtım(Inheritance)

Şablon Kalıtımı(Inheritance) ve Layoutlar
------------------------------------------

Çoğunlukla bir proje içerisindeki şablonlar site başlık bilgisi (header),
site alt bilgisi (footer) ya da yan kutular gibi pek çok genel öğeyi 
paylaşırlar. Symfony2'de biz bu konuya daha farklı açıdan baktık. Bir 
şablon başka bir şablon tarafından dekore edilebilir. Bu tamamen
PHP sınıfları gibidir. Şablon kalıtımı size "layout" (plan) 
adındaki, sitenin **bloklar** içerisinde tanımlanan (PHP'nin
metodlarını düşünün) genel öğelerini içeren bir şablon inşaa etmenize
olanak sağlar. Bir alt şablon bu ana layout şablonundan türetilerek
ana şablon içerisindeki bloklara hükmedebilir. (PHP alt sınıflarının
kendi üst sınıfın metodlarına hükmetmesini düşünün)

First, build a base layout file:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Test Application{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/blog">Blog</a></li>
                    </ul>
                    {% endblock %}
                </div>

                <div id="content">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Test Application') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar')): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Home</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    Şablon katılımı Twig'in terimleri içerisinde geçmesi rağmen,
    PHP ve Twig  şablonlarının  felsefesi aynıdır.

Bu şablon iki sütünlu basit bir şablonun HTML iskeletini tanımlar. Bu örnekte
üç ``{% block %}`` alanı tanımlanmıştır (``title``,``sidebar`` ve ``body``).
Her blok kendi içersinde ya da bir alt şablon tarafından hükmedilebilir.
Bu şablon direkt olarak da ekrana basılabilir. Bu durumda ``title``, ``sidebar`` 
ve ``body`` blokları şablonun varsayılan değerlerine sahip olacaklardır.

Bir alt şablon şu şekilde olabilir:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block title %}My cool blog posts{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'My cool blog posts') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   (``::base.html.twig``) özel stringi olarak tanımlanan üst
   şablon projenin ``app/Resources/views`` dizininde bulunan
   bir şablondur. Bu isimlendirme kuralı açık olarak 
   :ref:`template-naming-locations` 'nda açıklanmıştır
   
Şablon kalıtımı için ``{% extends %}`` etiketi kullanılır.
Bu şablon motoruna temel şablonda ilk layout'un ve tanımlanan bazı
blokların değerlendirmesini söyler. Alt şablon, üst şablondaki
``title`` ve ``body`` bloklarının alt şablon tarafından değişitirilmesinden
sonra ekrana basılır. ``blog_entries`` 'ın değerine göre çıktı şu şekilde
olabilir:

.. code-block:: html

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>My cool blog posts</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">Home</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>My first post</h2>
                <p>The body of the first post.</p>

                <h2>Another post</h2>
                <p>The body of the second post.</p>
            </div>
        </body>
    </html>

Dikkat edin; alt şablon ``sidebar`` bloğunu tanımlamadığında üst şablondaki
değer kullanılacaktır. ``{% block %}`` tagıyla belirlenen üst şablondaki içerik 
her zaman varsayılan değerdir.

Kalıtımı istediğiniz düzeyde kullanabilirsiniz. Sonraki kısımda genel bir 
üç düzeyli kalılıtım modelini Symfony2 projelerinde nasıl organize edileceği
açıklanmıştır.

Şablon kalıtımı ile çalışırken bazı hususları akılda tutmalısınız:

* Eğer şablonda ``{% extends %}`` kullanıyorsanız bu şablondaki ilk etiket olmalıdır.

* En güzeli ana şablonda fazla ``{% block %}`` etiketine sahip olmanızdır.
  Alt şablonlar tüm üst şablonların bloklarını tanımlamaz bu yüzden ana
  şablonunuzda ne kadar çok blok bulunursa, sablonunuz o kadar düzgün 
  ve esnek olacaktır.

* Eğer kendiniz pek çok şablon içerisinden içeriği çoğaltıyorsanız, muhtemelen bu
  içerikleri üst şablondaki ``{% block %}`` içerisine kaydırmalısınız.
  Bazı durumlarda en iyi çözüm içeriği yeni bir şablona aktarmak ve onu ``include``
  etmektir(içeri aktarmak). (bkz :ref:`including-templates`)

* Eğer üst şablonun blokundaki içeriği almak istiyorsanız,``{{ parent() }}``
  fonksyionunu kullanabilirsiniz. Bu fonksiyon içeriği yeniden düzenlemek
  yerine üst şablondan içeriği aktarmak için kullanışlı bir fonksiyondur:

    .. code-block:: html+jinja

        {% block sidebar %}
            <h3>Table of Contents</h3>
            ...
            {{ parent() }}
        {% endblock %}

.. index::
   single: Templating; İsimlendirme Kuralları
   single: Templating; Dosya Konumları

.. _template-naming-locations:

Şablon İsimlendirme ve Konumlandırma
------------------------------------

Varsayılan olarak şablonlar iki farklı konumda bulunabilirler:

* ``app/Resources/views/``: Uygulamalar ``views`` klasörü uygulama genelinde
  kullanılacak bundle şablonları tarafından hükmedilecek 
  (Örn. Uygulamanızın planı (layout)) ana şablonu barındırır
  (bkz :ref:`overriding-bundle-templates`);

* ``path/to/bundle/Resources/views/``: Her bundle kendi şablonlarını 
   ``Resources/views`` klasöründe saklar (ve alt klasörleri). Bundle içerisindeki
   şablonların önemli bir kısmı burada bulunur.

Symfony2 şablonlar için **bundle**:**controller**:**şablon** yazım 
şeklini kullanır. Bu belirtilen konumdaki farklı özelliklerde olan 
şablonların kullanılmasını mümkün kılar:

* ``AcmeBlogBundle:Blog:index.html.twig``: Bu yazın belirli bir sayfanın
  belirli bir şablonunu ifade eder.Bu string içerisinde iki nokta üstüste (``:``)
  ile ayrılmış üç alanın anlamı şudur:

    * ``AcmeBlogBundle``: (*bundle*) şablon 
      ``AcmeBlogBundle`` içerisinde bulunmaktadır. 
      (Örn: ``src/Acme/BlogBundle``);

    * ``Blog``: (*controller*) şablon ``Resources/views`` klasörünün altındaki
      ``Blog`` klasöründedir;

    * ``index.html.twig``: (*şablon*) ``index.html.twig`` dosyasının adı.

  ``AcmeBlogBundle`` 'ı ``src/Acme/BlogBundle`` içerisinde bulunduğunu varsaydığımızda
  ana plan (layut) 'un yolu ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``
  şeklinde olacaktır.

* ``AcmeBlogBundle::layout.html.twig``: Bu yazım şekli ``AcmeBlogBundle`` 'ın
  temel şablonunu ifade eder. Bu yazımda "controller" kısmı eksik olduğundan dolayı
  (Örn. ``Blog``) şablon  ``AcmeBlogBundle`` içindeki  ``Resources/views/layout.html.twig`` 
  konumundadır.

* ``::base.html.twig``: Bu yazım uygulama genelinde kullanılacak şablon ya da
  planı(layout) ifade eder.String'in iki adet iki nokta üstüste (``::``) ile
  başladığına ve *bundle* ve *controller* 'ların olmadığına dikkat edin.
  Bunun anlamı bu şablon herhangi bir bundle içerisinde konumlandırılmamakta, bunun
  yerine  ``app/Resources/views/``  klasöründe tutulmaktadır.

:ref:`overriding-bundle-templates` bölümünde ``AcmeBlogBundle`` içerisinde
her bir şablonun nasıl konumlandırıldığını ve mesela  
``app/Resources/AcmeBlogBundle/views/`` içerisinde aynı isimde olan şablonların
nasıl birbirlerine hükmettiklerini göreceksiniz.
Bu herhangi bir vendor bundle'ından size şablonlara hükmetme gücü verir.

.. tip::

    Umarız şablon adlandırmada kullanılan yazım şekli (syntax) 'ni anlamışsınızdır.
    Bu yazım şekli aynı :ref:`controller-string-syntax` yazım şekline benzemektedir.

Template Son Eki (Suffix)
~~~~~~~~~~~~~~~~~~~~~~~~~
**bundle**:**controller**:**template** formatı şablon dosyasının *nerede* 
konumlandırıldığını belirtir. Her şablon ismi ayrıca *format* ve *engine*
adındaki ekler ile belirtilir.

* **AcmeBlogBundle:Blog:index.html.twig** - HTML formatı, Twig motoru

* **AcmeBlogBundle:Blog:index.html.php** - HTML formatı, PHP motoru

* **AcmeBlogBundle:Blog:index.css.twig** - CSS formatı, Twig motoru


Varsayılan olarak herhangi bir Symfony2 şablonu PHP ya da Twig olarak yazılabilir 
ve uzantıyı belirten son kısım (örn. ``.twig`` ya da ``.php``) bu iki
motordan hangisinin *kullanılacağını* belirler. Uzantının ilk kısmı 
(örn. ``.html`` , ``.css`` , vs) yaratılacak içeriğin son tipini belirler.

Şablon motorudan farklı olarak Symfony2'nin şablonu nasıl yorumlayacağı
bu basit organizasyonel taktiği kullanmasıyla, aynı durumda kaynağın ihtiyaca göre 
HTML(``index.html.twig``) ya da XML(``index.xml.twig``) ya da diğer bir 
formatta ekrana basılacağını belirlenir.

Daha fazla bilgi için :ref:`template-formats` kısmını okuyun.

.. note::

   Mevcut "motorlar" konfigüre edilebildiği gibi yeni motorlar da eklenebilir.
   Daha fazla bilgi için :ref:`Şablon Konfigürasyonu'na <template-configuration>`
   bakınız.

.. index::
   single: Templating; Etiketler ve Yardımcılar (Helpers)
   single: Templating; Yardımcılar (Helpers)

Etiketler ve Yardımcılar (Helpers)
----------------
Şablonların temellerini ve bunların nasıl isimlendirilip nasıl birbirleri
ile ilişkilendirilebilleceğini artık öğrendiniz. En zor kısımı geride
bıraktınız. Bu kısımda diğer şablonları çağırmak, sayfalara link 
vermek ve resimleri göstermek gibi genel işlemlerde kullanılan, geniş
bir gurupta toplanan yardımcı araçları göreceksiniz.

Symfony2 şablon tasarımcısına yardımcı olmak amacıyla pek çok Twig etiketi ve 
fonksiyonu barındıran bundle'la birlikte gelir.PHP'de şablon sistemi 
şablon içeriğinde kullanılması için genişletilmiş bir  *yardımcı* 
(helper) sistemi sunar.

Gerçi biz zaten bir kaç Twig etiketi (``{% block %}`` & ``{% extends %}``) 
ve PHP için örneklerden gördüğümüz (``$view['slots']``) yardımcıları biliyoruz.
Şimdi biraz daha görelim.

.. index::
   single: Templating; Diğer Şablonları Çağırmak

.. _including-templates:

Diğer Şablonları Çağırmak
~~~~~~~~~~~~~~~~~~~~~~~~~
Sıklıkla farklı sayfalarda aynı şablonun ya da kodun bir kısmını kullanmak
isteyecekisni. Örneğin bir "haberler" kısmı olan bir uygulamada şablon kodu
haberi gösterebilmek için en çok izlenen haberlerde kullanılan haber detayı 
sayfasının bir kısmını kullanabilir ya da en son haberler listesinin bir 
kısmını kullanabilir. 

PHP kodunun bir kısmını kullanmak istediğinizde tipik olarak kodu yeni
bir PHP sınıfı ya da fonksiyonuna taşırsınız. Aynsı şablonlar için de 
geçerlidir. Şablon içerisinde kodu tekrar kullanmak için başka bir
şablondan çağırma yapılabilir. Öncelikle yeniden kullanacağımız kısım
için yeni bir şablon yapalım.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h2>{{ article.title }}</h2>
        <h3 class="byline">by {{ article.authorName }}</h3>

        <p>
            {{ article.body }}
        </p>

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.php -->
        <h2><?php echo $article->getTitle() ?></h2>
        <h3 class="byline">by <?php echo $article->getAuthorName() ?></h3>

        <p>
            <?php echo $article->getBody() ?>
        </p>

Bunu başka bir şablondan çağırmak oldukça basittir:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Recent Articles<h1>

            {% for article in articles %}
                {% include 'AcmeArticleBundle:Article:articleDetails.html.twig' with {'article': article} %}
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Recent Articles</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render('AcmeArticleBundle:Article:articleDetails.html.php', array('article' => $article)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

Şablon, diğer şablonu çağırmak için ``{% include %}``  etiketini kullanır.
Şablon isminin aynı tipik yazım şekline sahip olduğuna dikkat edin.
``articleDetails.html.twig`` şablonu ``article`` değişkenini kullanır.
Bu değişkenin değeri ``list.html.twig`` şablonundan  ``with`` komutu
ile aktarılır.

.. tip::

    ``{'article': article}`` yazımı Twig Standart karışık eşleştirme
    (hash maps)(örn. isimlendirilmiş anahtarlara sahip bir array (dize)) yazımıdır.
    Eğer birden fazla element aktarmak istediğimizde şu yazımı kullanırız:
    ``{'foo': foo, 'bar': bar}``.
    
.. index::
   single: Templating; Aksiyon gömmek (Embedding action)

.. _templating-embedding-controller:

Controller Gömmek (Embedding Controller)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bazı durumlarda bir şablonu çağırmaktan fazlasına ihityacını olur.
Varsayalım layout(plan) içerisinde en önemli üç haberi getiren bir sidebar(kenar bloku)
var. Bu üç haberin getirilmesinde belki şablondan yapılamayacak kadar
ağır bir işlem ya da bir veritabanı sorgusu gereki olabilir.
Çözüm ise basitçe şablonunuza sonucu getiren bir controller gömmektir.
İlk önce belirli bir sayıdaki haberi ekrana getiren controlleri yapalım:

.. code-block:: php

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // make a database call or other logic to get the "$max" most recent articles
            $articles = ...;

            return $this->render('AcmeArticleBundle:Article:recentList.html.twig', array('articles' => $articles));
        }
    }

``recentList`` şablonu gayet açık ve nettir:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="/article/{{ article.slug }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles as $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Dikkat ederseniz bu örnekte haber URL'sini doğrudan cheat(hile ile bozma) ettik.
    (Örn: ``/article/*slug*``). Ancak bu kötü bir örnek. Sonraki kısımda
    bunun nasıl düzeltilebileceğini göreceksiniz.

Controller'i çağırmak için controller'lar için kullanıan standart yazımı
kullanmanız gereklidir (Örn: **bundle**:**controller**:**aksiyon**):

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="sidebar">
            {% render "AcmeArticleBundle:Article:recentArticles" with {'max': 3} %}
        </div>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="sidebar">
            <?php echo $view['actions']->render('AcmeArticleBundle:Article:recentArticles', array('max' => 3)) ?>
        </div>

Şablona erişimi olmayan bir değişkenin bilgisini bulmanız gerektiğinde bir
controller'in bunu ekrana bastığını göz önünde bulundurun. 

Controller'lar çalıştırma, iyi kod organizasyonu sunma ve yeniden kullanım için
hızlı bir yol sunar.

.. index::
   single: Templating; Sayfalara Link Vermek

Sayfalara Link Vermek
~~~~~~~~~~~~~~~~~~~~~
Şablon içerisinde diğer sayfalara link vermek uygulamanız içerisinde en sık 
yapılan işlerden birisidir. URL'er üzerinde detaylıca çalışmak yerine 
``path`` Twig fonksiyonunu ( ya da PHP için ``router`` yardımcısı) kullanarak 
routing konfigürasyonunuza göre URL adreslerini yaratabilirsiniz.Sonra
eğer isterseniz sayfadaki URL'lerin bir kısmını değiştirmek isterseniz sadece
routing konfigürasyonunda değişiklik yaparak şablon içerisindeki URL'lerin
otomatik olarak yeniden yaratılmasını sağlayabilirsiniz.

Öncelikle aşağıdaki routing konfigürasyonundan ulaşılabilen "_welcome"
sayfasına link verelim:

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:  /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml

        <route id="_welcome" pattern="/">
            <default key="_controller">AcmeDemoBundle:Welcome:index</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Welcome:index',
        )));

        return $collection;

Sayfaya link vermek için sadece route'a işaret eden ``path`` Twig fonksiyonunu
kullanmanız yeterlidir:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_welcome') }}">Home</a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('_welcome') ?>">Home</a>

Beklenildiği üzere bu ``/`` URL'si yaratacaktır. Şimdi daha karışık bir
route üzerinde bunun nasıl çalıştığına bakalım:

.. configuration-block::

    .. code-block:: yaml

        article_show:
            pattern:  /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml

        <route id="article_show" pattern="/article/{slug}">
            <default key="_controller">AcmeArticleBundle:Article:show</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

Bu durumda route adı (``article_show``) ve değeri olan ``{slug}`` parametresinin
ikisini de belirtmelisiniz. Bu route'un kullanarak önceki kısımda gösterilen
``recentList`` şablonuna yeniden ziyaret ederek haberleri doğru bir şekilde
linklendirelim:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="{{ path('article_show', { 'slug': article.slug }) }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    Ayrıca mutlak URL adresini ``url`` Twig fonksiyonu ile de yaratabilirsiniz:

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    Aynısı ``generate()`` metodunun üç parametresini ayarlayarak da
    PHP şablonu içerisinde yapılabilir:

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('_welcome', array(), true) ?>">Home</a>

.. index::
   single: Templating; Varlıklara (asset) Link Vermek

Varlıklara (asset) Link Vermek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şablonlar genel olarak resimlere, Javascript'e, stil şablonlarına ve diğer
varlıkları da kullanır. Elbette bu varlıkların yollarını direkt olarak 
yazarak da (Örn. ``/images/logo.png``) kullanabilirsiniz ancak Symfony2 
``asset`` Twig fonksiyonu üzerinden daha dinamik bir seçenek sağlar.

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: html+php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

``asset``  fonksiyonunun asıl görevi uygulamayı daha taşınabilir yapmaktır.
Eğer uygulamanız host'unuzun kök'ünde ise (Örn: http://example.com), 
bu durumda ekrana bir varlık ``/images/logo.png`` şeklinde olacaktır. Ancak
eğer uygulamanız bir alt klasörde tutuluyorsa (Örn: http://example.com/my_app),
bu durumda her varlık alt klasör ile birlikte ekrana basılacaktır
(örn: ``/my_app/images/logo.png``).
``asset`` fonksiyonu uyglamanızın nasıl kullanıldığına bakarak doğru yollara
(path) göre bu varlıkların yollarını belirler. 

Ayrıca eğer ``asset`` fonksiyonunu kullanırsanız, Symfony otomatik olarak,
bu güncellenen statik varlıkların uygulamayı aktarma (deploy) esnasında ön belleğe
alınmaması için , bir sorgu stringini (query string) varlığınıza (asset) ekler.
Örneğin ``/images/logo.png`` , ``/images/logo.png?v2`` şeklinde gözükebilir. 
Daha fazla bilgi için :ref:`ref-framework-assets-version` konfigürasyon seçeneğine
bakın.

.. index::
   single: Templating; Javascript ve Stil Şablonlarını Aktarmak (Include)
   single: Stylesheets; Stil Şablonlarını Aktarmak (Include)
   single: Javascripts; Javascript Aktarmak (Include)

Twig içerisinde Javascript ve Stil Şablonlarını Aktarmak (Include)
------------------------------------------------------------------

Hiç bir site Javascript ve Stil Şablonları olmadan bitmiş sayılmaz.
Symfony'de bu varlıkların dahil edilmesi zarif bir şekilde Symfony'in
şablon kalıtım özelliği ile yapılır.

.. tip::

    Bu kısım size strin şablonları ve Javascript varlıklarının Symfony
    içerisine aktarılmasının (include) felsefesini öğretecek.
    Symfony ayrıca Assetic adında bu felsefeyi izleyen fakat bu varlıklar
    ile çok daha fazla ilginç sey yapmaya olanak veren bir kütüphane
    ile birlikte gelir. Assetic kullanımı hakkında daha fazla bilgi için
    :doc:`/cookbook/assetic/asset_management` belgesine bakın.


Varlıklarımızı barındırmak için ana şablonumuza ``head`` etiketi arasında 
olacak olan ``stylesheets`` adında ve diğeri de ``javascripts`` adında 
olan, ``body`` tagının hemen üzerinde,iki adet blok ekleyerek başlayalım.
Bu bloklar sitemizin tamamında ihtiyacımız olacak stil şablonlarını ve
javscript'leri barındırıcaklar:

.. code-block:: html+jinja

    {# 'app/Resources/views/base.html.twig' #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

Ne kadar kolay! Fakat eğer alt şablonda ekstra stilşablonu ya da Javascripte
ihtiyaç olursa ne olacak ? Örneğin, varsayalımki bir iletişim sayfanız var
ve *sadece* bu sayfada ``contact.css`` 'i kullanmanız gerekiyor.
İletişim sayfası şablonu içerisine aşağıdaki gösterilen kodu yazın :

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {% extends '::base.html.twig' %}

    {% block stylesheets %}
        {{ parent() }}
        
        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}
    
    {# ... #}

Alt şablonda basitçe ``stylesheets`` bloku'na hükmettiniz (override) ve
yeni stilşablonu etiketini bu blok içerisine koydunuz. Elbette üst
blokun içeriğinin değiştirilmesini istediğinizde (*replace* (değiştirme)
yapmadan), ``parent()`` Twig fonksiyonunu kullanarak ana şablondaki tanımlanan
``stylesheets`` blokunun içerisindeki herşeyi akatarabilirsiniz(include).

Ayrıca varlıkları bundle'ınız içerisindeki ``Resources/public`` klasöründen de
aktarabilirsiniz. Bunun için konsol üzerinden ``php app/console assets:install target [--symlink]``
komutunu çalıştırarak dosyalarınızı (ya da sembolink linklerini) doğru
konuma taşıyabilirsiniz. (target parametresindeki değer varsayılan olarak
"web" dir).

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" type="text/css" rel="stylesheet" />

Son olarak sonuç sayfası ``main.css`` ve ``contact.css`` stil şablonlarını
içeriye aktarmıştır.

Global Şablon Değişkenleri
---------------------------

Her istek esnasında Symfony2 Twig ve PHP şablon motorlarında 
varsayılan olarak ``app`` adındaki bir şablon global değişkenine değer 
atayacaktır. ``app`` değişkeni bazı uygulamaya özel değişkenleri
otomatik olarak veren  
:class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`
sınıfından türer:

* ``app.security`` - Güvenlik konusundaki içerik.
* ``app.user`` - Geçerli kullanıcı nesnesi.
* ``app.request`` - request nesnesi.
* ``app.session`` - oturum nesnesi.
* ``app.environment`` - Geçerli ortam (dev, prod, vs).
* ``app.debug`` - Hata Ayıklama Modu aktif ise True aksi halde False.

.. configuration-block::

    .. code-block:: html+jinja

        <p>Username: {{ app.user.username }}</p>
        {% if app.debug %}
            <p>Request method: {{ app.request.method }}</p>
            <p>Application Environment: {{ app.environment }}</p>
        {% endif %}

    .. code-block:: html+php

        <p>Username: <?php echo $app->getUser()->getUsername() ?></p>
        <?php if ($app->getDebug()): ?>
            <p>Request method: <?php echo $app->getRequest()->getMethod() ?></p>
            <p>Application Environment: <?php echo $app->getEnvironment() ?></p>
        <?php endif; ?>

.. tip::

    Eğer isterseniz kendi global şablon değişkenlerinizi de atayabilirsiniz.
    Bunun için :doc:`Global Değişkenler</cookbook/templating/global_variables>`
    adındaki tarif kitabı örneğine bakın.

.. index::
   single: Templating; Şablon Servisi

``templating`` Servisini Kullanmak ve Konfigüre etmek
------------------------------------------------------

Symfony2'nin şablon sisteminin kalbi templating ``Motoru`` dur.
Bu özel nesne şablonların ekrana basılması ve onların değerlerini
döndürmekten sorumlıdur. Örneğin bir controller içerisinden bir şablon 
ekrana bastığınızda, gerçekte şablon motor servisini kullanırsınız.
Örneğin:

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

ifadesi şuna eşittir.

.. code-block:: php

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

Şablon motoru (ya da "servisi") Symfony2 içerisinde otomatik olarak çalışması
için önceden ayarlanmıştır. Elbette bu başka bir uygulama konfigürasyon dosyasından da
ayarlanabilir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
            ),
        ));


Bazı konfigüraston seçenekekleri :doc:`Konfigürasyon Ekinde</reference/configuration/framework>`.
gösterilmiştir.

.. note::

   webprofiler kullanmak için ``twig`` motoru zorunludur (ve bazı 3. parti
   bundle'larda da bu olabilir).

.. index::
    single; Template; Şablonlara hükmetmek (override)

.. _overriding-bundle-templates:

Bundle Şablonlarına Hükmetmek (override)
----------------------------------------

The Symfony2 community prides itself on creating and maintaining high quality
bundles (see `KnpBundles.com`_) for a large number of different features.
Once you use a third-party bundle, you'll likely need to override and customize
one or more of its templates.

Suppose you've included the imaginary open-source ``AcmeBlogBundle`` in your
project (e.g. in the ``src/Acme/BlogBundle`` directory). And while you're
really happy with everything, you want to override the blog "list" page to
customize the markup specifically for your application. By digging into the
``Blog`` controller of the ``AcmeBlogBundle``, you find the following::

    public function indexAction()
    {
        $blogs = // some logic to retrieve the blogs

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

When the ``AcmeBlogBundle:Blog:index.html.twig`` is rendered, Symfony2 actually
looks in two different locations for the template:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

To override the bundle template, just copy the ``index.html.twig`` template
from the bundle to ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
(the ``app/Resources/AcmeBlogBundle`` directory won't exist, so you'll need
to create it). You're now free to customize the template.

This logic also applies to base bundle templates. Suppose also that each
template in ``AcmeBlogBundle`` inherits from a base template called
``AcmeBlogBundle::layout.html.twig``. Just as before, Symfony2 will look in
the following two places for the template:

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Once again, to override the template, just copy it from the bundle to
``app/Resources/AcmeBlogBundle/views/layout.html.twig``. You're now free to
customize this copy as you see fit.

If you take a step back, you'll see that Symfony2 always starts by looking in
the ``app/Resources/{BUNDLE_NAME}/views/`` directory for a template. If the
template doesn't exist there, it continues by checking inside the
``Resources/views`` directory of the bundle itself. This means that all bundle
templates can be overridden by placing them in the correct ``app/Resources``
subdirectory.

.. _templating-overriding-core-templates:

.. index::
    single; Template; Overriding exception templates

Overriding Core Templates
~~~~~~~~~~~~~~~~~~~~~~~~~

Since the Symfony2 framework itself is just a bundle, core templates can be
overridden in the same way. For example, the core ``TwigBundle`` contains
a number of different "exception" and "error" templates that can be overridden
by copying each from the ``Resources/views/Exception`` directory of the
``TwigBundle`` to, you guessed it, the
``app/Resources/TwigBundle/views/Exception`` directory.

.. index::
   single: Templating; Three-level inheritance pattern

Three-level Inheritance
-----------------------

One common way to use inheritance is to use a three-level approach. This
method works perfectly with the three different types of templates we've just
covered:

* Create a ``app/Resources/views/base.html.twig`` file that contains the main
  layout for your application (like in the previous example). Internally, this
  template is called ``::base.html.twig``;

* Create a template for each "section" of your site. For example, an ``AcmeBlogBundle``,
  would have a template called ``AcmeBlogBundle::layout.html.twig`` that contains
  only blog section-specific elements;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Blog Application</h1>

            {% block content %}{% endblock %}
        {% endblock %}

* Create individual templates for each page and make each extend the appropriate
  section template. For example, the "index" page would be called something
  close to ``AcmeBlogBundle:Blog:index.html.twig`` and list the actual blog posts.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Notice that this template extends the section template -(``AcmeBlogBundle::layout.html.twig``)
which in-turn extends the base application layout (``::base.html.twig``).
This is the common three-level inheritance model.

When building your application, you may choose to follow this method or simply
make each page template extend the base application template directly
(e.g. ``{% extends '::base.html.twig' %}``). The three-template model is
a best-practice method used by vendor bundles so that the base template for
a bundle can be easily overridden to properly extend your application's base
layout.

.. index::
   single: Templating; Output escaping

Output Escaping
---------------

When generating HTML from a template, there is always a risk that a template
variable may output unintended HTML or dangerous client-side code. The result
is that dynamic content could break the HTML of the resulting page or allow
a malicious user to perform a `Cross Site Scripting`_ (XSS) attack. Consider
this classic example:

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: html+php

        Hello <?php echo $name ?>

Imagine that the user enters the following code as his/her name::

    <script>alert('hello!')</script>

Without any output escaping, the resulting template will cause a JavaScript
alert box to pop up::

    Hello <script>alert('hello!')</script>

And while this seems harmless, if a user can get this far, that same user
should also be able to write JavaScript that performs malicious actions
inside the secure area of an unknowing, legitimate user.

The answer to the problem is output escaping. With output escaping on, the
same template will render harmlessly, and literally print the ``script``
tag to the screen::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

The Twig and PHP templating systems approach the problem in different ways.
If you're using Twig, output escaping is on by default and you're protected.
In PHP, output escaping is not automatic, meaning you'll need to manually
escape where necessary.

Output Escaping in Twig
~~~~~~~~~~~~~~~~~~~~~~~

If you're using Twig templates, then output escaping is on by default. This
means that you're protected out-of-the-box from the unintentional consequences
of user-submitted code. By default, the output escaping assumes that content
is being escaped for HTML output.

In some cases, you'll need to disable output escaping when you're rendering
a variable that is trusted and contains markup that should not be escaped.
Suppose that administrative users are able to write articles that contain
HTML code. By default, Twig will escape the article body. To render it normally,
add the ``raw`` filter: ``{{ article.body|raw }}``.

You can also disable output escaping inside a ``{% block %}`` area or
for an entire template. For more information, see `Output Escaping`_ in
the Twig documentation.

Output Escaping in PHP
~~~~~~~~~~~~~~~~~~~~~~

Output escaping is not automatic when using PHP templates. This means that
unless you explicitly choose to escape a variable, you're not protected. To
use output escaping, use the special ``escape()`` view method::

    Hello <?php echo $view->escape($name) ?>

By default, the ``escape()`` method assumes that the variable is being rendered
within an HTML context (and thus the variable is escaped to be safe for HTML).
The second argument lets you change the context. For example, to output something
in a JavaScript string, use the ``js`` context:

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formats

.. _template-formats:

Debugging
---------

.. versionadded:: 2.0.9
    This feature is available as of Twig ``1.5.x``, which was first shipped
    with Symfony 2.0.9.

When using PHP, you can use ``var_dump()`` if you need to quickly find the
value of a variable passed. This is useful, for example, inside your controller.
The same can be achieved when using Twig by using the debug extension. This
needs to be enabled in the config:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme_hello.twig.extension.debug:
                class:        Twig_Extension_Debug
                tags:
                     - { name: 'twig.extension' }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="acme_hello.twig.extension.debug" class="Twig_Extension_Debug">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Twig_Extension_Debug');
        $definition->addTag('twig.extension');
        $container->setDefinition('acme_hello.twig.extension.debug', $definition);

Template parameters can then be dumped using the ``dump`` function:

.. code-block:: html+jinja

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}

    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}


The variables will only be dumped if Twig's ``debug`` setting (in ``config.yml``)
is ``true``. By default this means that the variables will be dumped in the
``dev`` environment but not the ``prod`` environment.

Template Formats
----------------

Templates are a generic way to render content in *any* format. And while in
most cases you'll use templates to render HTML content, a template can just
as easily generate JavaScript, CSS, XML or any other format you can dream of.

For example, the same "resource" is often rendered in several different formats.
To render an article index page in XML, simply include the format in the
template name:

* *XML template name*: ``AcmeArticleBundle:Article:index.xml.twig``
* *XML template filename*: ``index.xml.twig``

In reality, this is nothing more than a naming convention and the template
isn't actually rendered differently based on its format.

In many cases, you may want to allow a single controller to render multiple
different formats based on the "request format". For that reason, a common
pattern is to do the following:

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

The ``getRequestFormat`` on the ``Request`` object defaults to ``html``,
but can return any other format based on the format requested by the user.
The request format is most often managed by the routing, where a route can
be configured so that ``/contact`` sets the request format to ``html`` while
``/contact.xml`` sets the format to ``xml``. For more information, see the
:ref:`Advanced Example in the Routing chapter <advanced-routing-example>`.

To create links that include the format parameter, include a ``_format``
key in the parameter hash:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            PDF Version
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Final Thoughts
--------------

The templating engine in Symfony is a powerful tool that can be used each time
you need to generate presentational content in HTML, XML or any other format.
And though templates are a common way to generate content in a controller,
their use is not mandatory. The ``Response`` object returned by a controller
can be created with our without the use of a template:

.. code-block:: php

    // creates a Response object whose content is the rendered template
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // creates a Response object whose content is simple text
    $response = new Response('response content');

Symfony's templating engine is very flexible and two different template
renderers are available by default: the traditional *PHP* templates and the
sleek and powerful *Twig* templates. Both support a template hierarchy and
come packaged with a rich set of helper functions capable of performing
the most common tasks.

Overall, the topic of templating should be thought of as a powerful tool
that's at your disposal. In some cases, you may not need to render a template,
and in Symfony2, that's absolutely fine.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`etiketler`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`filitreler`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`kendi eklentinizi dahi -yazabilirsiniz`: http://twig.sensiolabs.org/doc/extensions.html
