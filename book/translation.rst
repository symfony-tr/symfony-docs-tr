.. index::
   single: Tercümeler

Tercümeler
=========

"Uluslararasılaştırma" terimi (genellikle `i18n`_ olarak kısaltılır) 
özetlenmiş metinleri ve yerele özel uygulamanızdaki diğer parçaları işleme
ve kullanıcıların kendi yerel özelliklerine çevirebileceği,tercüme edebileceği
bir katmanı ifade eder (Örn : Dil bve ülke). Metin için bu, kullanıcının
dilinde, tercüme özelliği olan her metin (ya da "mesaj") 'i bir fonksiyon
ile paketlemeyi ifade eder::


    // metin *daima* İngilizce olarak gözükecetir.
    echo 'Hello World';

    // metin son kullanıcıların diline ya da İngilizceye tercüme edilebilir
    echo $translator->trans('Hello World');

.. note::

    *Yerel* (locale) terimi kabaca kullanıcıların dilini ve ülkesini ifade eder.
    Uygulamanızın kullandığı herhangi bir metini tercüme etmek ve diğer
    format farklılıkları (örn: para birimi formatı) yönetilebilir. 
    Biz `ISO639-1`_ *dil* kodu ve bir alt tire (``_``) ve daha sonrada 
    `ISO3166 Alpha-2`_ *ülke* kodunu tavsiye ediyoruz(Örn : Türkçe/Türkiye için ``tr_TR``).

Bu kısımda uygulamanın çoklu yerel özelliklere nasıl destek vereceği konusunda 
hazırlık yapmayı ve daha sonra da çoklu yerel özellikler için nasıl tercümeler
yaratılacağını öğreneceksiniz. Tüm bu aşamalar bazı genel adımlara sahiptir:

1. Symfony'nin ``Translation`` bileşenini aktif edip konfigüre etmek;

2. Özet karakter dizilerini ``Translator`` içerisinde çağırmak (örn : "mesajlar")  

3. Desteklenecek her yerel için bir tercüme kaynağı yaratarak bu yereller
   için her mesajı tercüme etmek;

4. Kullanıcılara atanacak yerel bilgilerini oturum(session) içerisine 
   aktarmak ve yönetme kısımlarını belirlemek.

.. index::
   single: Tercümeler; Konfigürasyon

Konfigürasyon
-------------

Tercümeler kullanıcıların yerellerini araştıran ve çevirilen mesajları 
döndüren ``Translator`` :term:`servisi` tarafından işlenir. Bunu kullanmadan
önce ``Translator`` 'u konfigürasyonunuz içerisinden aktif hale getirin:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

``fallback`` seçeneği, eğer kullanıcının yerel'ine göre bir çeviri bulunamadıysa
uygulanacak varsayılan dili ayarlamaya yarar.

.. tip::

    Yerel için bir tercüme bulunamadığında çevirici (translator) ilk 
    bulduğu dilin tercümesini deneyecektir (yerel örneğin ``tr`` ise
    ``tr_TR`` gibi). Eğer bu da başarısz olursa fallback parametresinde
    tanımlanan dili kullanacaktır.

Tercümelerde kullanılan yerel bilgisi kullanıcı oturumunda(session) saklanır.

.. index::
   single: Tercümeler; Temel Tercüme

Temel Tercüme
-------------

Metin tercümesi ``translator`` servisi ile yapılır
(:class:`Symfony\\Component\\Translation\\Translator`). Bir blok metni tercüme etmek
için (bu *mesaj* olarak adlandırılır) :method:`Symfony\\Component\\Translation\\Translator::trans`
metodunu kullanın. Varsayalım controller içerisindeki basit bir mesajı
tercüme edeceksiniz:
 
.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Bu kod çalıştırıldığında, Symfony2, "Symfony2 is great" mesajını kullanıcının
``locale`` bilgisine göre tercüme etmeye çalışacaktır. Bunun için Symfony2'ye 
mesajı,verilen yerel bilgisinin mesaj tecümeleri kolleksiyonu olan
"tercüme kaynağından" nasıl tercüme edeceğini söylememiz gereklidir. Bu 
tercüme "sözlüğü" farklı formatlarda yaratılabilir ancak tavsiye edilen XLIFF
formatıdır:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.tr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>Symfony2 harika</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.tr.php
        return array(
            'Symfony2 is great' => 'Symfony2 harika',
        );

    .. code-block:: yaml

        # messages.tr.yml
        Symfony2 is great: Symfony2 harika

Şimdi eğer kullanıcı yereli Türkiye ise (örn: ``tr_TR``), bu mesaj ``Symfony2 harika``
olarak tercüme edilecektir.

Tercüme Süreci
~~~~~~~~~~~~~~

Gerçekte mesajı tercüme etmek için Symfony2 basit bir süreç kullanır:

* Geçerli kullanıcının oturumda (session) saklanan, ``yerel`` (locale) bilgisi belirlenir;

* ``yerel`` (locale) için tanımlanan içerisinde tercüme edilmiş mesajların
  bulunduğu bir katalog yüklenir(Örn : ``tr_TR``). Varsayılan(fallback) yerel
  dili ayrıca katalog bulunamazsa ya da mevcut değilse yüklenir ve eklenir.
  Sonuç geniş bir tercüme "sözlüğü" dür. Daha fazla detay için `Mesaj Katalogları`_
  belgesine bakın;
  
* Eğer mesaj katalog içerisinde ise , tercümesi yapılır. Eğer yoksa çevirici
  (translator) orijinal mesajı döndürür.
  
``trans()`` metodu kullanıldığında, Symfony2 ilgili içerisindeki karakter
dizesini (string) araştırır ve katalog içerisindeki uygun mesajı döndürür 
(eğer var ise).

.. index::
   single: Tercümeler; Mesaj Yer tutucuları (placeholders)

Mesaj Yer Tutucuları
~~~~~~~~~~~~~~~~~~~~

Bazen içerisinde değişken olan bir mesaj çevrilmesi gerekebilir:

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

Ancak bu karakter dizisi(string) için tercüme yaparken çevirici(translator),  
içerisinde değişken değerinin olduğu bir mesajı tercüme etmesi imkansızdır
(Örn : "Hello Ryan" ya da "Hello Fabien"). ``$name`` Değişkenin alabileceği 
her değere göre bir tercüme yapmak yerine bu değişkeni bir "yer tutucu" 
(placeholder) içerisinde tanımlarız:

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 şimdi işlenmemiş mesajın (``Hello %name%``) tercümesine bakacak
ve *daha sonra* yer tutucuda berilenen değeri bu kısıma ekleyecektır. Bir
tercüme yaratılması ise aynı önceki gibidir:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.tr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Merhaba %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.tr.php
        return array(
            'Hello %name%' => 'Merhaba %name%',
        );

    .. code-block:: yaml

        # messages.tr.yml
        'Hello %name%': Merhaba %name%

.. note::

    Yer tutucular uzun mesajlar gibi herhangi bir formu PHP `strtr fonksiyonu`_
    ile yeniden yapılandırımış(reconstruction) şekilde alabilir. Bu yüzden
    ``%var%`` notasyonu çeviri yaparken tutarlı bir yapı sağlanması için 
    Twig şablonlarında bu gerekir.

Gördüğünüz gibi bir çeviri yapmak iki adımdan oluşur:

1. ``Translator`` tarafından işlenecek, tercüme edilecek bir özet mesaj.

2. Destek vermeyi istediğiniz her yerel(locale) için bu mesajın tercümesini
   yaratmak.
   
İkinci adım tercümeleri barındıran, farklı sayıdaki farklı yerel için tercümeleri
barındıran "mesaj katalogları" ile yapılır.

.. index::
   single: Tercümeler; Mesaj Katalogları

Mesaj Katalogları
-----------------

Bir mesajın tercümesinde Symfony2 mesaj katalogunu, kullanıcıların yereline
göre ve mesajın tercümesi araştırılması için derler. Bir mesaj katalogu
farklı yerel bilgisi için aynı bir sözlük gibidir. Örneğin. ``tr_TR``
yereli için bir katalog şu şekilde mesajın çevirisini barındıabilir:

    Symfony2 is Great => Symfony2 harika

Bu çok uluslu bir uygulamada yerelleştirme ile (ya da tercüme) ilgili 
geliştiricinin sorumluluğudur. Tercümeler Symfony tarafından taranan,
bazı kurallara göre oluşturulmuş bir dosya sistemi içerisinde saklanır.

.. tip::

    *Yeni* yaratacağınız her tercüme (ya da içerisinde tercüme kaynağı
    barındıran bir bundle kurulumu) kaynağında Symfony2'nin yeni
    tercüme kaynaklarını bulabilmesi için ön belleği (cache) temizleyin:
    
    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Tercümeler; Tercüme kaynak konumları

Tercüme Konumları ve İsimlendirme Kuralları
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 mesajlar için (tercümeler) iki konuma bakar:

* Bundle içerisinde bulunan mesajlar için ilgili mesaj dosyaları
  bundle'ın ``Resources/translations/`` klasöründe bulunmalıdır;

* Herhangibir bundle tercümesini değiştirmek(override) için mesaj
  dosyalarını ``app/Resources/translations`` klasörüne yerleştirin.

Symfony2'nin tercümelerin detaylarını belirlerken kullandığı kurallar yüzünden
tercüme dosyalarının isimleri ayrıca önemlidir. Her mesaj dosyasının adı şu 
şablona göre verilmelidir: ``domain.locale.loader`` :

* **domain**: Mesajların gurup halinde organize edilmesi için isteğe bağlı bir
  kısımdır(Örn: ``admin``,  ``navigation`` ya da varsayılan olarak ``messages``).
  bkz. `Mesaj Domain'lerini Kullanmak`_
  
* **locale**: tercümelerin yapılacağı yerel bilgisi (örn: ``en_GB``, ``en``, vs...)

* **loader**: Symfony'nin dosyayı nasıl yorumlayacağını ve yükleyeceğini 
  belirten kısım (örn: ``xliff``, ``php`` ya da ``yml``).

loader kısmı kayıtlı herhangi bir yükleyici ismi olabilir. Varsayılan olarak
Symfony aşağıdaki yükleyicileri sağlar:

* ``xliff``: XLIFF dosyası;
* ``php``:   PHP dosyası;
* ``yml``:  YAML dosyası.

Yükleyici olarak hangisini kullanacağınız sizin zevkinize kalmıştır.

.. note::

    Ayrıca çevririleri bir veri tabanında ya da :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` 
    interface'inden türetilmiş özel bir sınıf aracılığı ile de kayıt altına alabilirsiniz.
    

.. index::
   single: Tercümeler; Tercüme kaynakları yaratmak

Tercümeleri Yaratmak
~~~~~~~~~~~~~~~~~~~~

Tercüme dosyaları yaratma işinin önemli bir parçası "yerelleştirme" dir
(sıklıkla `L10n'_ olarak kısaltılır). Tercüme dosyaları verilen ad ve
yerel bilgisine göre bir dizi id-tercüme eşleşmesine sahiptir. Kaynak,
özgün tercümenin tanımlayıcısı, ana yerelin mesajı (e.g.
"Symfony is great") ya da benzersiz bir tanımlayıcı olabilir 
(e.g."symfony2.great" - aşağıda kutu bilgisine bakın):


.. configuration-block::

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/translations/messages.tr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>Symfony harika</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>Symfony harika</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/translations/messages.tr.php
        return array(
            'Symfony2 is great' => 'Symfony harika',
            'symfony2.great'    => 'Symfony harika',
        );

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/translations/messages.tr.yml
        Symfony2 is great: Symfony harika
        symfony2.great:    Symfony harika

Symfony2 bu dosyaları araştıracak ve ya "Symfony2 is great" ya da 
"symfony2.great" 'ı Türkçe diline tercümede kullanacaktır (örn: ``tr_TR``).


.. sidebar:: Gerçek ya da Anahtar Kelime Mesajlarını Kullanmak

    Bu örnek çeviri olacak mesajların yaratımında iki ayrı felsefeyi açıklamaktadır:
    

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    İlk metodda mesajlar varsayılan yerel dilde yazılmaktadır (Bu durumda
    İngilizce). Bu mesaj tercümeler yaratılırken id olarak kullanılacaktır.

    İkinci metodda mesajlar mesajı iletecek "anahtar kelimeler" olarak tanımlanır.
    Anahtar mesaj herhangi bir tercüme için id olarak kullanılır.Bu durumda tercümeler
    varsayışlan yerelde tercüme edilmelidir (örn. Ana yerel Türkçe ise 
    ``symfony2.great`` , ``Symfony2 harika`` olarak tercüme edilmelidir).

    İkinci metod eğer mesajı ana yerel dilde "Symfony2 gerçekten harika"
    olarak okunmasına karar verdiysek,anahtar kelimenin her tercüme dosyasında 
    değiştirilmeye ihtiyacı olmadığından dolayı daha pratiktir. 

    Hangi metodu kullanacağınız tamamen size kalmış. Ancak "anahtar kelime"
    metodu  daha çok önerilir.

	Ayrıca ``php`` ve ``yaml`` dosya formatları eğer id'leriniz için
	geçek metinler kullanıyorsanız kendinizi sürekli tekrarlamamak için
	iç içe geçmiş (nested) id'lere destek verir:
	
	
    .. configuration-block::

        .. code-block:: yaml

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great' => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    Çoklu düzeyler  her düzey arasında tek bir id/tercüme çiftine nokta (.) ile
    birleştirilebildiğinden dolayı aşağıdaki örnekler de aynı anlama gelir:
    

    .. configuration-block::

        .. code-block:: yaml

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: Tercümeler; Mesaj domain'leri

Mesaj Domain'lerini Kullanmak
-----------------------------

Gördüğümüz gibi mesaj dosyaları farklı yerellerde tercüme edilmiş şekilde
organize olmuş dosyalardır. Mesaj dosyaları ayrıca "domain" 'ler içerisinde
mesaj dosyaları yaratılırken domain dosyanın ilk kısmı olacak şekilde de 
organize edilebilir. Varsayılan domain ``messages`` dır. Örneğin organize
etmek amacıyla domainler üç ayrı formata bölünsün. ``messages``, ``admin``
ve ``navigation`` . Türkçe tercüme dosyaları aşağıdaki şekilde olacaktır:

* ``messages.tr.xliff``
* ``admin.tr.xliff``
* ``navigation.tr.xliff``

Karakter dizileri tercüme edilirken varsayılan domain (``messages``) içerisinde
değilse bunu ``trans()`` 'ın üçüncü argümanı olarak tanımlamalısınız:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 şimdi kullanıcı yerelinde mesaj için ``admin`` domain'ine bakacaktır.

.. index::
   single: Tercümeler; Kullanıcının Yereli

Kullanıcının Yerel Bilgisini İşlemek
-------------------------------------

Gerçerli kullanıcının yerel bilgisi oturum (session) içerisinde saklanır
ve ``session`` servisi aracılığı ile erişilebilir:

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Tercümeler; Varsayılan Yerel

Varsayılan Yerel Bilgisi
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Eğer yerel bilgisi oturum(session) içerisinde açıkça belirtilmediyse,
``Translator`` tarafından ``fallback_locale`` konfigürasyon 
parametresindeki değer kullanılır. Bu parametrenin varsayılan değeri ``en`` dir
(`Konfigürasyon`_ 'a bakın).

Alternatif olarak kullanıcının yerel bilgisini oturum içerisinde tanımlanmasını
garanti altına almak istiyorsanız oturum servisi için ``default_locale``
bilgisini tanımlamalısınız:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: { default_locale: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session default-locale="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array('default_locale' => 'en'),
        ));

.. _book-translation-locale-url:

Yerel Bilgisi ve URL
~~~~~~~~~~~~~~~~~~~~

Kullanıcının yerel bilgisi oturum (session) içerisinde saklandığından dolayı
aynı URL'yi kullanıcının yerel bilgisine göre farklı kaynaklar kullanarak
göstermek isteyebilirsiniz. Örneğin ``http://www.example.com/contact``
içeriği bir kullanıcıda İngilizce, diğer bir kullanıcıda Türkçe
olabilir. Maalesef bu Webin ana kuralını ihlal eder. Bu URL adresi,
kullanıcıya dikkat etmeden tek kaynaktan kendini geri döndürür. Daha beter
bir sorunsa içeriğin hangi sürümü arama motorları tarafından indeksleneceği
sorunudur.

En iyi çözüm ise yerel bilgisini URL içerisine almaktır. Bu route sistemi
tarafından desteklenen özel ``_locale`` parametresi ile sağlanır:

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:   /{_locale}/contact
            defaults:  { _controller: AcmeDemoBundle:Contact:index, _locale: en }
            requirements:
                _locale: en|tr|de

    .. code-block:: xml

        <route id="contact" pattern="/{_locale}/contact">
            <default key="_controller">AcmeDemoBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|tr|de</requirement>
        </route>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Contact:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|tr|de'
        )));

        return $collection;

Özel `_locale` parametreini route içerisinde kullanıldığında eşleşen
yerel bilgisi *otomatik olarak kullanıcını oturumuna kayıt edilecektir*.
Başka bir ifade ile eğer kullanıcı ``/tr/contact`` URI 'sini ziyaret ederse,
yerel ``tr`` otomatik olarak kullanıcının yerel bilgisi olarak oturumuna
kayıt edilecektir.

Şimdi uygulamanız içerisindeki diğer tercüme edilmiş sayfaları göstermek için 
kullanıcının yerel bilgisini route üzerinde yaratabiliyorsunuz.

.. index::
   single: Tercümeler; Çoğulluk

Çoğulluk (Pluralization)
------------------------

.. note::

	Ç.N: Bu kısım Türkçe dil bilgisi kuralları arasında yer almadığından
	Türkçe'den başka dil kullanmayacaksanız bu kısmı atlayın. Ancak
	Uygulamanızda İngilizce, Almanca, Fransızca, Rusça gibi dillere destek
	verecekseniz bu kısmı okuyabilirsiniz.


Mesajların çoğul halleri kuralları oldukça karmaşık olabildiğinden zor bir 
konudur. Bu yüzden Rusça çoğul haller kurallarını matematiksel gösterimi
bulunmaktadır::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Gördüğünüz gibi Rusçada her birisine 0,1 ya da 2 indexi verilen üç 
farklı çoğul hal bulunmaktadır. Her çoğul hal farklı ve bu yüzden de çeviride
farklı olmaktadır.

Tercümenin çoğul hallerden dolayı farklı formları olduğunda bir karakter
dizisinin tüm çoğul halleri  bir komut borusu ile ayrılabilir (``|``)::

    'There is one apple|There are %count% apples'

Çoğullaştırılmış mesajları kullanmak için 
:method:`Symfony\\Component\\Translation\\Translator::transChoice` metodunu
kullanın:

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

İkinci argüman (bu örnekte ``10``) nesnelerin tanımlandığı ve kullanılacak
tercümede doldurulacak olan ``%count%`` yer tutucusunun *sayısıdır*.

Verilen sayıya göre translator, doğru çoğul hali seçer.
Ingilizcede çoğu kelimenin tekil hali sadece bir nesneyi ifade ederken çoğul
hali diğer sayılar için (0,2,3...) tanımlanır. Bu yüzden eğer ``count``
değişkeni ``1`` ise translator ilk tercümede 
(``There is one apple``) olan ilk karakter dizisini kullanacaktır. Aksi takdirde 
 ``There are %count% apples`` şeklini kullanacaktır.
 
Burada Fransızca tercümesi vardır::

    'Il y a %count% pomme|Il y a %count% pommes'

Karakter dizileri birbirine çok benzese bile (bu iki alt metin komut borusu ile
birbirinden ayrılmıştır), Fransızca kuralları farklıdır. İlk form (çoğul olmayan)
``count`` değeri ``0`` ya da ``1`` için kullanılır. Bu yüzden translator otomatik
olarak ``count`` değeri ``0`` ya da ``1`` olduğunda ilk karakter dizisini 
(``Il y a %count% pomme``) kulanacaktır.

Her yerelin kendisine göre en az altı farklı çoğul hali ile hangi sayıların hangi çoğul
halde kullanılacağını belirleyen kuralları ile birlikte berlileyen kuralları vardır.
Kurallar İngilizce ve Fransızca için oldukça basit iken Rusça için hangi kuralın
hangi karakter dizisi ile kullanılacağı için bazı yardımlar almalısınız.
Tercümanlara yardımcı olabilmek için isteğe bağlı olarak her karakter dizisi
için "etiket" kullanabilirsiniz::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Etiketler tercümanlar için oldukça iyi yardımcılardır ve tercümanların
hangi çoğul hali nerede kullanacağı hakkında onlara yardımcı olur.
Etiketler tanımlayıcı herhangi bir iki nokta üstüste ile biten (``:``) bir
karakter dizisi olabilir. Etiketler ayrıca orijinal mesajla aynı olmak zorunda
değildir.

.. tip:

    Etiketler isteğe bağlıdır ve tercüman istemezse bunları kullanmaz
    (tercüman sadece karkater dizisini etiketten sonraki ilk 
    pozisyonuna bakar).

Verilen Aralıkları Çoğullamak (Explicit Interval Pluralization)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bir mesajı çoğul hale getirmenin en kolay yolu Symfony2 'nin kullandığı 
verilen sayıya göre hangi çoğul formu kullanacağını seçen iç algoritmayı
kullanmaktır. Baze belirli tercümelerde daha fazla kontrol etmeyi isteyip 
farklı tercümeler yapmak isteyebilirsiniz(``0`` ya da adet sayısı negatif ise).
Bu durumlar için açık matematik aralıklarını(explicit math intervals) kullanın ::

    '{0} There are no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Aralıklar `ISO 31-11`_ notasyonunu kullanır. Yukarıdaki karakter dizisi 
dört farklı aralık tanımlar: eşittir ``0``, eşittir ``1``,``2-19`` ve ``20``
ve yukarısı.

Ayrıca eşitlik matematik kuralları ile standart kurallarıda harmanlayabilirsiniz.
Bu durumda eğer adet sayısı belirli bir aralıkla eşleşmezse standart kurallar
devreye girerler::

    '{0} There are no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Örneğin ``1`` elma(apple) için, standart kural ``There is one apple`` kullanılacaktır.
``2-19`` arası elma için iklinci standart kural `There are %count% apples`` 
seçilecektir.

:class:`Symfony\\Component\\Translation\\Interval` sınıfı sonlu bir sayı kümesini
ifade edebilir::

    {1,2,3,4}

Ya da iki sayı arasındaki sayıları::

    [1, +Inf[
    ]-1,2[

Sağ ayraç ``[`` (içinde) ya da ``]`` (dışında) olabilir.
Sol ayraç ``[`` (dışında) ya da ``]`` (içinde) olabilir. Sayıların dışında
``-Inf`` ve ``+Inf`` sonsuzluk ifadelerinide kullanabilirsiniz.

.. index::
   single: Tercümeler; Şablonlarda Tercümeler

Şablonlar içerisinde Tercümeler
-------------------------------

Çoğu zaman tercümeleri şablonlar içerisinde de görülür. Symfony2 Twig ve PHP
şablonları için bunu doğal olarak destekler.

Twig Şablonları
~~~~~~~~~~~~~~~

Symfony2 *statik metin blokları* 'nın çevirisinde yardımcı olan özel Twig
etiketleri(``trans`` ve ``transchoice``) sağlar:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There are no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}


``transchoice`` etiketi otomatik olarak ``%count%`` değişkenini geçerli
içerikten alır ve translator'a gönderir. Bu mekanizma sadece ``%var%``
deseninde tanımlanan yer tutucularda çalışır.

.. tip::

    Eğer bir metin dizesi (string) içerisinde yüzde (``%``) işaretini kullanmanız
    gerekiyorsa bu karakteri çiftleyerek kullanabilirsiniz:
    ``{% trans %}Yüzde: %percent%%%{% endtrans %}``

Ayrıca mesaj domaini tanımlayarak bazı ek değişkenlerde aktarabilirsiniz:

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% trans with {'%name%': 'Fabien'} from "app" into "fr" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

``trans`` and ``transchoice`` filitreleri *değişken metinleri* ve karmaşık
ifadelerde de kullanılabilir:


.. code-block:: jinja

    {{ message|trans }}

    {{ message|transchoice(5) }}

    {{ message|trans({'%name%': 'Fabien'}, "app") }}

    {{ message|transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Tercüme etiketleri ya da filitreleri kullanmak bir ince farkla 
    aynı etkiyi yaratır: otomatik kaçış(escaping) sadece bir filitre
    kullanıldığında değişkenlere uygulanır. Başka bir ifade ile 
    eğer tercüme edilen değişkenin çıktısın için *kaçış karakteri* 
    kullanılmaması gerekiyorsa bu durumda tercüme yapıldıktan sonra
    "raw" filitresi uygulanması gerekir:

    .. code-block:: jinja

            {# etiketler (h3) içerisindeki metin ayıklanmayacak #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# varsayılan olarak değişken filitre ile tercüme edildiğinde ayıklanacak #}
            {{ message|trans|raw }}

            {# fakat sabit karakter dizileri asla ayıklanmaz #}
            {{ '<h3>foo</h3>'|trans }}

PHP Şablonları
~~~~~~~~~~~~~~

PHP şablonlarından Translator servisi ``translator`` yardımcısı ile 
ulaşılabilir:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Translator Yerelini Değiştirmek
--------------------------------

Symfony2 mesaj tercümesinde kullanıcı oturumundaki yerel bilgisini ya da
gerekiyorsa ``fallback`` yerel bilgisini kullanır. Ancak kullanılacak 
yerel bilgisini manuel olarak da ayarlayabilirsiniz:

.. code-block:: php

    $this->get('translator')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'fr_FR',
    );

    $this->get('translator')->transChoice(
        '{0} There are no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'fr_FR',
    );

Veritabanı İçeriğini Tercüme Etmek
----------------------------------

Veritabanı içeriğini Doctrine üzerinden tercüme etmek için `Translatable Extension`_
'unu kullanmanız gerekir. Daha fazla bilhgi için bu kütüphanenin belgelerine
bakın.

.. _book-translation-constraint-messages:

Kısıt Mesajlarını Tercüme Etmek
-------------------------------

Kısıtl mesajlarını tercüme etmeyi öğrenmenin yolu bunu bir uygulama üzerinde
görmektir. Varsayalım uygulamanızın herhangi bir yerinde kullanmak üzere 
düz bir PHP nesnesi yarattınız:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

-Desteklenen metodlardan herhangi birisiyle kısıtları ekleyin. ``message`` seçeneğini
tercüme kaynak metni olarak belirleyin. Örneğin $name sınıf değişkeninin boş
geçilemeyeceğini belirlemek için şunu ekleyin:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: { message: "author.name.not_blank" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank(message = "author.name.not_blank")
             */
            public $name;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank">
                        <option name="message">author.name.not_blank</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank(array(
                    'message' => 'author.name.not_blank'
                )));
            }
        }

Kısıt mesajları için genelde bundle içerisinde ``Resources/translations/``
klasörü altında bir ``validators`` tercüme kataloğu yaratın.
Daha fazla bilgi için `Mesaj Katalogları`_ kısmına bakın.

.. configuration-block::

    .. code-block:: xml

        <!-- validators.tr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>author.name.not_blank</source>
                        <target>Yazar ismi boş olamaz.</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // validators.tr.php
        return array(
            'author.name.not_blank' => 'Yazar ismi boş olamaz.',
        );

    .. code-block:: yaml

        # validators.tr.yml
        author.name.not_blank: Yazar ismi boş olamaz.

Özet
----

Symfony2 Translation bileşeni ile uluslararası bir uygulama yaratmak bir kaç
adımı kapsayan zahmetsiz, ağrısız bir süreçtir:

* Özet mesajlarınızı ya 
  :method:`Symfony\\Component\\Translation\\Translator::trans` ile ya da
  :method:`Symfony\\Component\\Translation\\Translator::transChoice` metodları
  ile tanımlayın.

* Birden fazla yerel için her mesajın tercümesini yerel tercüme mesaj dosyaları yaratarak
  sağlayın. Symfony2 özel bir isimlendirme kuralına göre arama yaptığından dosyaları
  bu kurala göre adlandırın;

* Kullanıcıların oturum (session) içerisinde yer alan yerel bilgisini 
  yönetin.

.. _`i18n`: http://en.wikipedia.org/wiki/Internationalization_and_localization
.. _`L10n`: http://en.wikipedia.org/wiki/Internationalization_and_localization
.. _`strtr fonksiyonu`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`ISO3166 Alpha-2`: http://en.wikipedia.org/wiki/ISO_3166-1#Current_codes
.. _`ISO639-1`: http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
