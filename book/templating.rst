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
Symfony2 topluluğu kendisiyle faklı özellikteki bir çok bundle'ı 
(bkz `KnpBundles.com`_) yarattığı ve destek sağladığı için gurur duyuyor.

Bir kere 3. parti bir bunle kullandınız mı muhtemelen bu bundle'in
bir ya da daha fazla şablonunu düzenlemek ihtiyacı hissedeceksiniz.

Varsayalım ki açık kaynak kodlu ``AcmeBlogBundle``  adlı hayali bir
bundle'ı uygulamanızda kullancaksınız(``src/Acme/BlogBundle`` klasöründeki).
Herşeyden mutlu bir durumda iken blog'un "liste" sayfasını uygulamanıza
göre değiştirmek istediniz. ``AcmeBlogBundle`` 'ın ``Blog``  controller'ini
kurcalarken şunu buldunuz::

    public function indexAction()
    {
        $blogs = // blogları getiren bazı mantıksal süreçler.

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }


``AcmeBlogBundle:Blog:index.html.twig`` ekrana basıldığındı, Symfony2 
gerçekte şablon için iki farklı yere bakar:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``


Bundle şablonuna hükmetmek (override) için sadece bundle'daki 
``index.html.twig`` dosyasını ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
(eğer ``app/Resources/AcmeBlogBundle`` klasörü yoksa bunu yaratmanız gerekecek.)
kopyalamanız yeterlidir. Artık şablonu dilediğiniz gibi değiştirebilirsiniz.

Bu mantık ayrıca temel bundle şablonlarına da uygulanabilir. Yine varsayalım 
ki ``AcmeBlogBundle`` bundle'ının içindeki her şablon ``AcmeBlogBundle::layout.html.twig``
den kalıtımla(inherit) türetiliyor. Az önceki gibi Symfony2 şablon için iki
ayrı yere bakacaktır:

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Yine, şablona hükmetmek için (override) sadece bunu bundle içerisinden
``app/Resources/AcmeBlogBundle/views/layout.html.twig`` kopyalamanız
yeterlidir. Artık şablonu dilediğiniz gibi değiştirebilirsiniz.

Bir adım geri gittiğimizde Symfony2'nin her zaman şablon için 
``app/Resources/{BUNDLE_NAME}/views/`` klasörüne baktığını göreceksiniz.
Eğer burada şablon yok ise bundle içerisinde bulunan ``Resources/views``
klasörüne bakılacaktır. Bunun anlamı tüm bundle şablonları doğru
``app/Resources`` alt dizinleri içerisinden hükmedilebilir(override).

.. _templating-overriding-core-templates:

.. index::
    single; Template; İstisna Şablonlarına Hükmetmek (override)

Çekirdek Şablonlara Hükmetmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 frameworkunun kendisinin sadece bir bundle olmasından dolayı,
çekirdek şablonlar aynı yolla değiştirilebilir. Örneğin ``TwigBundle`` çekirdeği'nin
birden fazla farklı "istisna(exception)" ve "hata" şablonunu değiştirmek için 
her birisini ``TwigBundle`` 'ın ``Resources/views/Exception``  klasöründen alıp
sizinde tahmin ettiğiniz gibi ``app/Resources/TwigBundle/views/Exception``  
içerisine koyabilir, buradan değişikliklerinizi yapabilirsiniz.

.. index::
   single: Templating; Üç düzeyli kalıtım şablonu(inheritance pattern)

Üç düzeyli kalıtım(inheritance)
-------------------------------

Kalıtımı kullanmak için genel bir yol, üç düzeyli yaklaşımı kullanmaktır.
Bu metod birazdan değineceğimiz üç farklı tipteki şablon için mükemmel
şekilde çalışır:

* Uygulamanızın ana planını (önceki örnekteki gibi)
  içeren (layout) bir ``app/Resources/views/base.html.twig`` dosyası yaratın.
  İçsel olarak bu şablon ``::base.html.twig`` olarak adlandırılır;

* Sitenizin her "kısmı" için bi şablon yaratın. Örneğin ``AcmeBlogBundle``
  için sadece bloga özel nesneleri içeren ``AcmeBlogBundle::layout.html.twig``
  adında bir şablon;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Blog Application</h1>

            {% block content %}{% endblock %}
        {% endblock %}

* Her sayfa için bağımsız şablonlar yaratın ve uygun kısım şablonu ile genişletin.
  Örneğin güncel blog girdilerini gösteren, ``AcmeBlogBundle:Blog:index.html.twig`` 
  gibi bir isimde olan bir "index" sayfası.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Dikkat ederseniz bu şablon sırasıyla ilgili kısım şablonundan, 
(``AcmeBlogBundle::layout.html.twig``) o da ana uygulama şablonu (``::base.html.twig``)
üzerinden türetilmiştir. İşte bu genel olarak üç düzeyli kalıtım modeli olarak
adlandırılır.

Bir uygulama geliştirirken, bu metodu kullanabilirsiniz ya da basitçe
her sayfayı ana uygulama şablonundan direkt genişletirsiniz
(Örn: ``{% extends '::base.html.twig' %}``).Üç-Şablon modeli 
ana şablonun kolaylıkla değiştirilebildiği , bundle'ın şablonlarının 
uygulama'nın ana şablonuna basitçe adapte edilebildiği vendor bundle'larında kullanılan
bir metodtur.

.. index::
   single: Templating; Çıktıyı temizlemek (Output escaping)

Çıktıyı temizlemek (Output escaping)
------------------------------------

Şablon üzerinden HTML yaratımında her zaman şablon değişkenlerinin istenmeyen
HTML ya da tehlikeli istemci-tarafı(client-side) kod olarak çıktı vermesi 
riski vardır. Bunun sonucunda dinamik içeriğin çıktısı olan HTML bozulabilir ya da
kötü niyetli bir kullanıcı `Cross Site Scripting`_ (XSS) atağı deneyebilir.
Şu klasik örneği inceleyelim:

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: html+php

        Hello <?php echo $name ?>

Eğer kullanıcı kendi adı olarak aşağıdaki kodu girerse ne olur?::

    <script>alert('hello!')</script>

Eğer çıktıyı herhangi bir şekilde temizlemezsek (escaping) şablonda sonuç
olarak bir Javascript dikkat penceresi belirecektir:


    Hello <script>alert('hello!')</script>

Bu belki zararsız gibi gözükebilir ancak aynı kötü niyetli kullanıcı
kullanıcı eğer isterse bu güvenli alanda kötü niyetli bir kod yazarak 
meşru bir kullanıcı gibi bilinmeyen bir şeyler yapabilir.

Bu problemin cevabı çıktıyı temizlemektir (output escaping).
Çıktıyı temizleme esnasında şablon zararsız bir şekilde ekrana basılacak
ve ``script`` etiketi harfi harfine gözükecektir::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Twig ve PHP şablonlama sistemleri bu konuya farklı açılardan yaklaşır.
Eğer Twig kullanıyorsanız çıktı temizlenmesi varsayılan olarak yapılır
ve güvende olursunuz. PHP'de çıktı gtemizlenmesi otomatik olmadığından bunu
gerektiği yerde manuel olarak yapmanız gerekir.

Twig'de Çıktıyı Temizleme
~~~~~~~~~~~~~~~~~~~~~~~~~

Eğer Twig kullanıyorsanız çıktı temizlenmesi varsayılan olarak yapılır.
Bunun anlamı, kullanıcının gönderdiği kod yüzünden ortaya çıkacak istenmeyen
sonuçlardan her zaman korunmanızdır. Genel olarak çıktı temizleme
içeriğin HTML çıkışı için temizlenmesi olarak anlaşılmalıdır.

Bazı durumlasrda çıktı temizlemeyi bir değişkenin değerini gerçekten güvenip
temiz olduğuna inandığınız için iptal etmet isteyebilirsiniz. Varsayalımki
yönetici özelliğindeki kullanıcılar HTML kodu içeren haberler yazabilsinler.
Varsayılan olarak, Twig haber metninin gövdesinde bunları temizleyecektir.
Bunların normal halleri ile ekrana basılması için ``raw`` filitresi kullanılır:
``{{ article.body|raw }}`` .

Ayrıca çıktı temizlemeyi ``{% block %}``  alanı içerisinde ya da tüm
şablon için kapatabilirsiniz. Bu konuda daha fazla bilgi için Twig
dokümanları içerisindeki `Çıktı Temizleme`_  kısmına bakın

PHP'de Çıktı Temizleme
~~~~~~~~~~~~~~~~~~~~~~

PHP şablonlarında çıktı temizleme otomatik olarak yapılmaz. Bunun anlamı
bir değişkenin değerini temizleme ihtiyacı hissetmedikçe korumalı değilsiniz 
demektir. Çıktı temizlemeyi kullanmak için ``escape()`` görünüm metodunu
kullanırsınız::


    Hello <?php echo $view->escape($name) ?>


Varsayılan olarak ``escape()`` metodu bu değişkenin HTML içeriği ile ekrana
basılacağını varsayar (ve bu değişklen HTML için güvenli hale getirilecek
şekilde temizlenir). 

İkinci argüman ise ise içeriği değiştirmeye izin verir. Örneğin içeriğinizde
``js`` içeriğini kullanan bir kaç Javascript çıktısı için ::

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Templating; Formatlar

.. _template-formats:

Hata Ayıklama
--------------

.. versionadded:: 2.0.9
    Bu özellik Symfony 2.0.9 ile birlikte gelen Twig ``1.5.x`` sürümleri
    için geçerlidir.

PHP kullanırken değişkene atanan değeri hızlı bir şekilde görebilmek için
``var_dump()`` fonksiyonunu kullanabilirsiniz. Örneğin controller içerisinde
Bu kullanışlıdır. Aynı şeyi Twig içerisinde hata ayıklama eklentisi ile de
yapabilirsiniz. Bunun için konfigürasyon içerisinden bu özelliği aktif etmelisiniz:

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

Şablon parametreleri ``dump`` fonksiyonu ile içerikleri görülebilir:

.. code-block:: html+jinja

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}

    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}


Değişkenler sadece eğer Twig'in ``debug`` ayarı (``config.yml`` deki) ``true``
olursa içerikleri gösterilecektir. Varsayılan olarak bunun anlamı değşkenler
``dev`` ortamında içerikleri gözükebilir, ``prod`` ortamında değil.

Şablon Formatları
-----------------

Şablonlar içeriği *herhangi bir * formatta ekrana basmak için kullanılan
genel bir yoldur. Bir şablon sadece kolay bir şekilde JavaScript, CSS, XML
ya da düşünebildiğiniz her hanhangi bir formatta içerik yaratmaktan çok,
çoğu durumlarda HTML içeriğini basmak için kullanılır. 

Örneğin, aynı "kaynak" sıklıkla farkı formatlarda ekrana basılır. Haberlerin
index sayfasını XML'de ekrana basmak için basitçe şablon ismine formatın ismi
eklenir:

* *XML şablon adı*: ``AcmeArticleBundle:Article:index.xml.twig``
* *XML şablon dosyası adı*: ``index.xml.twig``

Gerçekte, bu bir isimlendirme kuralından başka bir şey değildir ve bunun
böyle olması onun ilgili formatta olacağı anlamına gelmez.

Pek çok durumda "istek format" 'ına göre tek bir controller kullanarak
farklı tipteki formatları ekrana basmak isteyebilirsiniz. Bu yüzden 
aşağıdaki genel bir şablon(pattern) bu işlemi gerçekleştirir:

.. code-block:: php


    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

``Request``  nesnesinin ``getRequestFormat`` da varsayılan ``html``
 dir ancak , kullanıcının isteğine göre başka format'ta dönebilir.

İstek formatı (request format) genellikle route tarafından kontrol edilir.
Bu yüzden  ``/contact`` değeri request format'ı ``html`` yaparken 
``/contact.xml`` değeri request formatı ``xml`` yapar. 
Daha fazla bilgi için :ref:`Routing kısmındaki ileri düzey örneklere bakın <advanced-routing-example>`.

Format parametresini içeren bir link yaratmak için parametre öbeğinin 
içerisine  ``_format`` değişkenini eklemelisiniz:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            PDF Version
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Son Düşünceler
--------------

Şablon motoru Symfony de HTML, XML ya da diğer formattaki içeriği istediğiniz
her zaman yaratabilecek güçlü bir araçtır. Şablonlar controller içerisinden
içeriği yaratmadaki en sık kullanılan yol olsa da şablonların kullanımı 
zorunlu değildir. Controller tarafından döndürülen ``Response`` nesnesi 
şablon kullanarak ya da kullanmayarak bu içeriği yaratabilir::

.. code-block:: php

    // ekrana basılacak bir şablonla birlikte yaratılan bir Response Nesnesi
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    //Basit bir içeriğe sahip olan bir Response Nesnesi
    $response = new Response('response content');


Symfony'nin şablon motoru iki adet farklı tipte olan,
geleneksel *PHP* şablonları ve zarif ve güçlü *Twig* şablonlarını varsayılan 
olarak ekrana basabilen oldukça esnek bir motordur.
İkiside pek çok genel işi kolaylıkla halledebilecek yardımcı metodlar ve
zengin bir fonksiyon seti ile birlikte gelmektedir.

Sonuç olarak şablon konusu kullanmayabileceğiniz ancak güçlü bir araç
olarak bilmeniz gereken bir konudur. Bazı durumlarda Symfony2'de hiç
şablon kullanma ihtiyacını hissetmeyebilirsiniz. Bu da çok doğaldır.

Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Çıktı Temizleme`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`etiketler`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`filitreler`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`kendi eklentinizi dahi -yazabilirsiniz`: http://twig.sensiolabs.org/doc/extensions.html
