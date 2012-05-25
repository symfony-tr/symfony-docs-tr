.. index::
   single: Formlar

Formlar
=======
HTML formları ile uğraşmak bir web geliştiricisinin en sık yaptığı -ve
cebelleştiği- işlerden birisidir. Symfony2 formlarla uğraşmayı kolaylaştıracak
bir Form bileşeni ile birlikte gelir. Bu bölüm boyunca sıfırdan form kütüphanesinin
en önemli özellikleri ile karmaşık formlar yapmayı öğreneceksiniz.

.. note::

   Symfony form bileşeni Smfony2 projeleri haricinde de kullanılabilecek
   kendi başına çalışan bir kütüphanedir. Daha fazla bilgi için Github'daki
   `Symfony2 Form Bileşeni`_ 'ne bakın.

.. index::
   single: Forms; Basit bir form yaratmak

Basit Bir Form Yaratmak
-----------------------
Varsayalımki ihtiyacımız doğrultusunda yapacağımız işleri gösteren
basit bir yapılacaklar (todo) listesi uygulaması geliştireceksiniz. 
Kullanıcılarınızın görevleri yaratacak ve düzenleyecek olmalarından ötürü
sizin bir form geliştirmeniz gerekecek. Başlamadan önce tek bir "görevi"
temsil eden ve saklayan ``Task`` adındaki bir sınıfa odaklanalım:

.. code-block:: php

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;

    class Task
    {
        protected $task;

        protected $dueDate;

        public function getTask()
        {
            return $this->task;
        }
        public function setTask($task)
        {
            $this->task = $task;
        }

        public function getDueDate()
        {
            return $this->dueDate;
        }
        public function setDueDate(\DateTime $dueDate = null)
        {
            $this->dueDate = $dueDate;
        }
    }

.. note::

   Eğer bu örneği bütün bir örnek halinde kodlayacaksınız ``AcmeTaskBundle``
   bundle'ını aşağıdaki komutu kullanarak yaratın(ve tüm varsayılan
   ayarları kabul edin):

   .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TaskBundle

Bu sınıf "düz-eski-PHP-nesnesi" olduğundan dolayı Symfony ile ya da 
diğer bir kütüphane ile hiç bir şey yapmayacaktır. Bu oldukça basit normal
bir PHP nesnesi uygulamanız içerisindeki sorunu *direkt* olarak çözer
(öen: bir görevi uygulamanızda temsil etmesi açısından). Elbette bu 
bölümün sonunda ``Task`` tipinde bir veriyi gönderecek, doğruluğunu
kontrol edecek ve veritabanında saklayacaksınız.

.. index::
   single: Forms; Controller içerisinde bir form yaratma

Form Geliştirme
~~~~~~~~~~~~~~~

Şimdi bir ``Task`` sınıfı yarattınız, diğer adım ise güncel bir HTML formu
yaratmak ve ekrana basmak. Symfony2'de bu form objesini geliştirerek ve
şablon içerisinde ekrana basarak olur. Şimdilik bunların tamamını
controller içerisinden yapabiliriz::

    // src/Acme/TaskBundle/Controller/DefaultController.php
    namespace Acme\TaskBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Acme\TaskBundle\Entity\Task;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function newAction(Request $request)
        {
            // bir görev yarat ve bu örnek için bazı örnek veriler ver
            $task = new Task();
            $task->setTask('Write a blog post');
            $task->setDueDate(new \DateTime('tomorrow'));

            $form = $this->createFormBuilder($task)
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->getForm();

            return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

.. tip::

   Bu örnek size formların direkt controller içerisinden nasıl yaratılacağını
   gösterir. Sonra  ":ref:`book-form-creating-form-classes`" kısmında 
   form'ların kendi başına bir sınıf halinde nasıl geliştirilip yeniden 
   kullanılabilir hale getirebileceğinizi öğreneceksiniz.
   

Bir formu yaratmak görece olarak Symfony2 form objesini yaratan
bir "form yapıcı (builder)" ile olduğundan dolayı daha az kod yazılır.
Form builder'lar basit form "reçeteleri" yazmak ve bir formu geliştirirken
bütün ağır işleri kaldırmaktan sorumludur.

Bu örnekte formunuza ``task`` ve ``dueDate`` adında ``Task`` sınıfını
``task`` ve ``dueDate`` değişkenleri ile ilişkili iki adet alan eklediniz.
Ayrıca her birisine bu alanların ekrana basılması esnasında hangi HTML
etiket(ler)'ini kullanılacağını belirlemek için bir tip atadınız
(örn. ``text``, ``date``). 

Symfony2 burada kısaca değileceğimiz pek çok ön tanımlı hazır tip ile
birlikte gelir (bkz :ref:`book-forms-type-reference`).  

.. index::
  single: Forms; Basit şablon renderi(ekrana basma)

Formu ekrana basma (render)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Madem ki form yaratıldı, sonraki adım onu ekrana basmak. Bu şablonunuz
içerisindeki özel bir form "gösterm" nesnesine aktarılarak 
(yukarıdaki controller içerisindeki ``$form->createView()`` ifadesine
dikkat edin) ve bir dizi form yardımcı fonksiyonu aracılığı ile yapılır:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?> >
            <?php echo $view['form']->widget($form) ?>

            <input type="submit" />
        </form>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    Bu örnek önceden ``task_new`` adındaki bir route'u ve
    ``AcmeTaskBundle:Default:new` 'ı işaret eden bir controller 
    yarattığınızı varsayar.

Bu kadar! ``form_widget(form)`` ile ekrana basarken her form alanı bir etiket
ve bir hata mesajı ile (Eğer hata oluşursa) yaratılacaktır. Çok kolay olmasına
rağmen bu çok esnek değil (henuz). Genellikle eyer form alanını formu nasıl
görünmesini istediğiniz şekilde ayrı ayrı olarak ekrana basmak istersiniz.
Bunun nasul yapılacağını ":ref:`form-rendering-template`" kısmında
öğreneceksiniz.

Devam etmeden önce ``$task`` (Örn "Write a blog post") 
nesnesinin ``task`` değişkenindeki değerin ``task`` metin kutusu halinde 
ekrana nasıl basıldığına dikkat edin.
Formun ilk işi nesneden veriyi almak ve onu HTML formu olarak basmak için
uygun bir formata çevirmektir.

.. tip::

   Form sistemi ``Task``  sınıfından protected özellikteki  ``task`` değişkeninden
   veriye ``getTask()`` ve ``setTask()`` metodları ile erişecek kadar zekidir.
   Sınıf değişkeni public özellikte olmadıkça bunların form bileşeninin
   veriyi alması ve vermesi için *mutlaka* "getter" ve "setter" fonksiyonlarının
   olması gerekir. Mantıksal(boolean) bir değişken için "getter" 
   (Örn. ``getPublished()``) metodu yerine "isser" (Örn. ``isPublished()``) 
   metodunu kullanmanız gerekir.
   
.. index::
  single: Forms; Form gönderisini işlemek

Form Gönderilerini İşlemek
~~~~~~~~~~~~~~~~~~~~~~~~~~

Formun ikinci görevi kullanıcının gönderdiği verileri nesnenin değişkenlerine
aktarmaktır. Bunu gerçekleştirmek için, kullanıcının verilerini forma 
girmesi gerekmektedir. Controller'ınıza aşağıdaki özelliği ekleyin::

    // ...

    public function newAction(Request $request)
    {
        // sadece temiz bir $task nesnesi yarat (yapay verileri temizle)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);

            if ($form->isValid()) {
                // veritabanına "görev" verisini yazmak gibi bazı işlemleri yap

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        // ...
    }

Şimdi form verisi gönderildiğinde (submit) controller gönderilen form verisini
form üzerinden alır, ``$task`` nesnesinin ``task`` ve ``dueDate`` değişkenlerine
aktarmak için çevirir. Bunların hepsi ``bindRequest()`` metodu ile olur.

.. note::

    ``bindRequest()`` çağırılır çağırılmaz veri hemen temel nesneye transfer edilir.
    Temel veri, gerçekte geçerli olsun ya da olmasın bu gerçekleşir.

Bu controller formları işlerken genel olarak, olası üç yolla şu şablonu izler:

#. Sayfa tarayıcıda yüklenmeye başladığında istek metodu ``GET`` tir ve form
   basitçe yaratılır ve ekrana basılır

#. Kullanıcı form verilerini hatalı veriler ile gönderdiğinde 
   (örn. ``POST`` metodu ile) form verileri alır ve daha sonra ekrana basılır. 
   Bu esnada tüm doğrulama hataları gösterilir;

#. Kullanıcı form verilerini doğru veriler ile gönderdiğinde, form bunları alır
   ve bu verileri bazı işlemler yapma fırsatı sağlamak amacıyla (örn: veriyi 
   veritabanına yazmak gibi) ``$task`` nesnesine kullanıcı başka bir 
   sayfaya yönlendirilmeden önce aktarılır(örn: "teşekkürler" ya da "başarılı" sayfası gibi).

.. note::

   Kullanıcı başarılı form gönderisinden sonra yönlendirilmeden önce
   kullanıcı veriyi yeniden göndermek için "refresh" özelliğine sahip olacaktır.
   
.. index::
   single: Forms; Veri Doğrulama

Form Verisi Doğrulama
----------------------

Önceki kısımda form verilerinin nasıl geçerli ya da yanlış verilerle 
gönderilebileceğiniz gördünüz. Symfony2'de veri doğrulama temel 
nesneye uygulanır (Örn: ``Task`` nesnesi). Diğer bir ifade ile soru
"form"'un geçerli olup olmaması ya da ``$task`` nesnesinin 
form verisi gönderildikten sonra nesne içerisindeki verinin kontrol 
edilip edilmemesi değildir. ``$form->isValid()`` metodu çağırıldığında 
``$task`` nesnesine bunun geçerli bir veri olup olmadığı sorulur.

Veri doğrdulama sınıfa eklenen bir dizi kuralla (kısıtlar) yapılır.
Bunu uygulamada görmek için ``task`` form alanına boş olamayacağını ve 
``dueDate`` form alanına boş olamayacağı ve geçerli bir \DateTime 
nesnesi türünde olmasını söyleyecek doğrulama kısıtını koyalım.

.. configuration-block::

    .. code-block:: yaml

        # Acme/TaskBundle/Resources/config/validation.yml
        Acme\TaskBundle\Entity\Task:
            properties:
                task:
                    - NotBlank: ~
                dueDate:
                    - NotBlank: ~
                    - Type: \DateTime

    .. code-block:: php-annotations

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Task
        {
            /**
             * @Assert\NotBlank()
             */
            public $task;

            /**
             * @Assert\NotBlank()
             * @Assert\Type("\DateTime")
             */
            protected $dueDate;
        }

    .. code-block:: xml

        <!-- Acme/TaskBundle/Resources/config/validation.xml -->
        <class name="Acme\TaskBundle\Entity\Task">
            <property name="task">
                <constraint name="NotBlank" />
            </property>
            <property name="dueDate">
                <constraint name="NotBlank" />
                <constraint name="Type">
                    <value>\DateTime</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        class Task
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('task', new NotBlank());

                $metadata->addPropertyConstraint('dueDate', new NotBlank());
                $metadata->addPropertyConstraint('dueDate', new Type('\DateTime'));
            }
        }


Bu kadar! Eğer formunuzu geçersiz veriler ile yeniden gönderirseniz form 
üzerinde ilgili yerlerde hata mesajlarını göreceksiniz.

.. _book-forms-html5-validation-disable:

.. sidebar:: HTML5 Veri Doğrulaması

   HTML5 ile birlikte pek çok tarayıcı belirli veri doğrulama kısıtlarını 
   istemci tarafında doğal olarak yapabilmektedir. En çok kullanılan 
   veri doğrulama türü gerekli form alanlarının ``required`` ifadesi
   ile gösterilmesidir. HTML5 destekleye tarayıcılar için bunun sonucu 
   eğer kullanıcı formun bu alanını boş bıraktığında bir tarayıcı
   hatası gibi gözükecektir.

   Yaratılan formlar bu yeni özelliğin tüm avantajlarını kullanarak
   hassas HTML5 nitelikleri ile veri doğrulama özelliği eklenmektedir.
   İstemci tarafı veri doğrulama aynı zamanda form etiketine ``novalidate`` 
   niteliği eklemek ya da submit etiketine ``formnovalidate`` özelliği
   eklemekle kapatılabilir.Bu özellikle sunucu tarafı veri doğrulama araçlarını
   test etmek istediğinizde oldukça kullanışlıdır.
   

Veri doğrulama Symfony2'nin oldukça güçlü bir özelliğidir ve kendi
:doc:`ayrılmış bir bölüm içerisinde anlatılmaktadır</book/validation>`. 

.. index::
   single: Forms; Veri Doğrulama Gurupları

.. _book-forms-validation-groups:

Veri Doğrulama Gurupları
~~~~~~~~~~~~~~~~~~~~~~~~

.. tip::

    Eğer :ref:`veri doğrulama gurupları <book-validation-validation-groups>` 'nı
    kullanmıyorsanız bu kısımı geçin.

Eğer nesneniz :ref:`veri doğrulama gurupları <book-validation-validation-groups>` 'nın
avantajlarını kullanmasını istiyorsanız formunuz içerisinde kullanmanız gereken veri
doğrulama gurup(lar)ını tanımlamanız gerekecektir::

    $form = $this->createFormBuilder($users, array(
        'validation_groups' => array('registration'),
    ))->add(...)
    ;

Eğer :ref:`form sınıfları <book-form-creating-form-classes>` 
yaratıyorsanız (en iyi uygulama), bu durumda ``getDefaultOptions()`` 
metodunda bunları eklemeniz gerekir:: 


    public function getDefaultOptions(array $options)
    {
        return array(
            'validation_groups' => array('registration')
        );
    }


Her iki durumda da *sadece* ``registration`` veri doğrulama gurubu temel
nesnede veri doğrulamak için kullanılacaktır.

.. index::
   single: Forms; Hazır Alan Tipleri

.. _book-forms-type-reference:

Hazır Alan Tipleri
--------------------

Symfony karşılaşacağınız tüm genel form alan ve veri tipleri için geniş
bir alan tipi gurubu ile standart olarak gelir:

.. include:: /reference/forms/types/map.rst.inc

Eğer isterseniz kendinize özel alan tipleri de yaratabilirsiniz. Bu konu
tarif kitabının ":doc:`/cookbook/form/create_custom_field_type`" adlı 
makalesinde işlenmiştir.

.. index::
   single: Forms; Alan tipi seçenekleri

Alan Tipi Seçenekleri
~~~~~~~~~~~~~~~~~~~~~

Her alan tipi konfigüre edebilmek için bir dizi seçeneğe sahiptir.
Örneğin ``dueDate`` alanı normalde 3 seçim kutusu ile ekrana basılır.
Ancak :doc:`tarih alanı</reference/forms/types/date>` tek bir giriş
kutusu (input box) olacak şekilde(kullanıcı tarihi metin tipinde girmek
isteyebilir) ayarlanabilir:

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Her alan tipinin aktarılabilecek bir dizi farklı seçeneği vardır.
Bunların pek çoğu alan tipine özeldir ve her alan tipinin dokümantasyonu
içerisinde bulunabilir.

.. sidebar:: ``required`` seçeneği

    En genel seçenek olan ``required`` seçeneği herhangi bir alana uygyulanabilir.
    Varsayılan olarak ``required`` seçeneği eğer alan boş ise bunu 
    HTML5-hazır tarayıcıların istemci tarafı doğrulama yapması için
    ``true`` değerine sahiptir.
    Eğer bunun browser tarafından yapılmasını istemiyorsanız ya ``required``
    alanının değerini ``false`` yapacaksınız ya da 
    :ref:`HTML5 veri doğrulamasını kapatacaksınız<book-forms-html5-validation-disable>`.

	Ayrıca ``required`` seçeneğinin ``true`` olması sunucu tarafı veri doğrulamada
	geçerli **olmayacağını** unutmayın. Diğer bir ifade dile eğer kullanıcı bu alan
	için boş bir değer verirse (örneğin, eski bir tarayıcı ya da bir web servisi ile)
	bu değer ilgili alanda Symfony'nin ``NotBlank`` ya da  ``NotNull`` kısıtlarını 
	kullanılmadıkça geçerli sayılacaktır.

    Kısaca ``required`` seçeneği "güzel" ancak sunucu tarafı veri doğrulama
    *daima* kullanılmalıdır.

.. sidebar:: ``label`` seçeneği

    Herhangi bir form alanının etiketi için ``label`` seçeneği kullanışabilir::

        ->add('dueDate', 'date', array(
            'widget' => 'single_text',
            'label'  => 'Due Date',
        ))

    Bu alan için etiket aşağıda görüldüğü gibi ,
    ayrıca formun ekrana basıldığı şablonda da ayarlanabilir.

.. index::
   single: Forms; Alan tipi tahmini

.. _book-forms-field-guessing:

Alan Tipi Tahmini
-------------------

Şimdi ``Task`` sınıfına veri doğrulama bilgisi eklediniz ve Symfony
artık bu alan için bir şeyler biliyor. Eğer kabul ederseniz,
Symfony alan tipinizi "tahmin edebilir" ve sizin için ayarlayabilir.
Bu örnekte Symfony ``task`` ve ``dueDate`` alanlarının ikisi için
tanımlanmış veri doğrulama kısıtlarından ne olduklarını tahmin ederek
``metin`` ve  ``tarih`` alanlarını oluşturur::

    public function newAction()
    {
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task')
            ->add('dueDate', null, array('widget' => 'single_text'))
            ->getForm();
    }



``add()`` metodunun ikinci argümanını atladığınız zaman (ya da 
eğer ``null`` yaparsanız) "tahminleme" aktif olacaktır. Eğer üçüncü argüman
olarak seçenekleri bir array değişkeni şeklinde verirseniz (bu seçenekler yukarıda
``dueDate`` için yapılmıştır) bu seçenekler tahmin edilen
alana uygulanacaktır.

.. caution::

    Eğer formunuz özel bir veri doğrulama gurubu kullanıyorsa alan tipi
    tahmincisi alan tiplerinin tahmin edildiğinde *tüm* diğer veri doğrulama kısıtlarını 
    dikkate almıyor olacaktır(bu kısıtlar veri doğrulama gurubu tarafından
    kullanılacak olan kısıtlara dahil olmayacaktır).

.. index::
   single: Forms; Alan tipi tahmini

Alan Tipi Seçenekleri Tahmini
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Alanın tipine  göre tahminlemeye ek olarak Symfony ayrıca alan bir dizi
alan tipi seçeneğinin doğru değerlerini de tahmin edebilir.

.. tip::

    Bu seçeneklere değerleri atandığında alan, HTML5 'in istemci tarafı 
    veri doğrulamasını içeren özel HTML nitelikleri ile ekrana basılacaktır.
    Bu, sunucu tarafı veri doğrulama kısıtlarını da yaratmayacaktır
    (Örn. ``Assert\MaxLength``). Sunucu tarafı veri doğrulamayı elden 
    düzenlemek zorunda olmanıza rağmen bu alan tipi seçenekleri bu bilgi
    kullanılarak tahmin edilebilecektir.
    

* ``required``: ``required`` seçeneği veri doğrulama kısıtlarından 
   ya da doctrine tanımlama verisi üzerinden tahmin edilebilir 
   (Örn. alan ``NotBlank`` ya da ``NotNull`` ise ) 
   Eğer istemci tarafı veri doğrulama otomatik olarak veri doğrulama 
   kurallarını eşleyecekse bu oldukça kullanışlıdır.

* ``max_length``: Eğer alan bir dizi metin alanı ise , bu durumda 
  ``max_length`` seçeneği veri doğrulama kısıtlarından (eğer ``MaxLength``
  ya da ``Max`` kullanıldı ise) ya da Doctrine tanımlama verilerinden(metadata)
  (alanın uzunluğu (length) üzerinden) tahmin edilebilir.
  
.. note::

  Bu alan seçenekleri tahmini *sadece* eğer Symfony alan tiplerini
  kullanıyorsanız olacaktır (Örn. ``add()`` metodunun ikinci argümanını es geçer ya da ``null`` yaparsanız)

Eğer tahmin edilen bir değeri değiştirmek isterseniz bu değer için
alan tanımlamalarındaki seçenekler array'ine değer bindirerek yapabilirsiniz::

    ->add('task', null, array('max_length' => 4))

.. index::
   single: Forms; Şablonda Ekrana Basmak

.. _form-rendering-template:

Şablondan Bir Formu Ekrana Basmak
---------------------------------

Şimdiye kadar tüm form'un nasul sadece bir satır kod ile ekrana  basılacağını
gördünüz. Elbette genellikle ekrana basılma esnasında daha fazla esneklik 
isteyeceksiniz:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}

            {{ form_rest(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- // src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->

        <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?>>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>

            <?php echo $view['form']->rest($form) ?>

            <input type="submit" />
        </form>

Her kısmı tek tek inceleyelim:

* ``form_enctype(form)`` - Eğer form içerisinde sadece bir alan bile dosya 
  yükleme (upload) tipinde olsa bile bu alan zorunlu olarak 
  ``enctype="multipart/form-data"`` şeklinde ekrana basılır;

* ``form_errors(form)`` - Formun tamamı için genel hata mesajlarını gösterir
  (Alana özel hata mesajları alandan hemen sonra gösterilir;

* ``form_row(form.dueDate)`` - belirli bir form nesnesi için varsayılan olarak ``div``
  elementi içerisinde, Etiketi , herhangi bir hata mesajı ve HTML kodu (Örn: ``dueDate``)
  ekrana basılır.

* ``form_rest(form)`` - Formun ekrana basılmamış kalan tüm öğelerini ekrana basar.
  Bu helper (yardımcı) genellikle formun altında ekrana basılmayı unutulan 
  tüm öğeleri ekrana bastığı için oldukça kullanışlıdır ( bazı durumlarda 
  ekrana hidden (gizli) alanlarını kod içerisinde göstermek istemeyebilir ya da
  unutabilirsiniz). Bu yardımcı aynı zamanda otomatik olarak :ref:`CSRF Korumasını<forms-csrf>`
  da sağlar.

``form_row`` yardımcısı genel olarak her form alanı için, etiketi, hataları 
ve HTML form nesnesinin kodunu varsayılan olarak ``div`` etiketi içerisinde 
yapar. :ref:`form-theming` kısmında ``form_row`` çıktısının farklı düzeylerde
nasıl özelleştirilebileceğini göreceksiniz.

.. tip::

    ``form.vars.value`` değişkeni üzerinden formun güncel verisine de
    erişebilirsiniz.

    .. configuration-block::

        .. code-block:: jinja

            {{ form.vars.value.task }}

        .. code-block:: html+php

            <?php echo $view['form']->get('value')->getTask() ?>

.. index::
   single: Forms; Her alanı elden düzenlemek

Her Alanı Elden Düzenlemek
~~~~~~~~~~~~~~~~~~~~~~~~~~~
``form_row`` formunuzdaki her alanı cok çabuk bir şekilde 
ekrana basabildiğinden dolayı mükemmel bir yardımcıdır("row" için
bu işaretleme aynı zamanda özelleştirilebilir de)
Fakat hayat bu kadar kolay olmadığından dolayı ayrıca her alanı
elden de hazırlanamanız gerekebilir. Bu durum aşağıdaki ``form_row``
helperi kullanılarak yapılan koda benzer:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_errors(form) }}

        <div>
            {{ form_label(form.task) }}
            {{ form_errors(form.task) }}
            {{ form_widget(form.task) }}
        </div>

        <div>
            {{ form_label(form.dueDate) }}
            {{ form_errors(form.dueDate) }}
            {{ form_widget(form.dueDate) }}
        </div>

        {{ form_rest(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->errors($form) ?>

        <div>
            <?php echo $view['form']->label($form['task']) ?>
            <?php echo $view['form']->errors($form['task']) ?>
            <?php echo $view['form']->widget($form['task']) ?>
        </div>

        <div>
            <?php echo $view['form']->label($form['dueDate']) ?>
            <?php echo $view['form']->errors($form['dueDate']) ?>
            <?php echo $view['form']->widget($form['dueDate']) ?>
        </div>

        <?php echo $view['form']->rest($form) ?>

Eğer alan için otomatik yaratılan etiket tam olarak doğru değil ise 
bunu açık bir şekilde belirleyebilirsiniz: 

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Bazı alan tipleri ekrana basılırken ekstra seçeneklerin düzenlenmesine
ihtiyaç duyabilir. Bu seçenekler her alan tipi belgesinden açıklanmıştır
ancak en genel seçenek , form elementinin bir niteliğini değiştirmeye 
izin veren ``attr`` seçeneğidir.
Aşağıda ``task_filed``  adında , metin kutusuna bir css stili eklenecektir:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, { 'attr': {'class': 'task_field'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Eğer "manuel" olarak form alanlarını düzenlemeyi düşünüyorsanız her alan
için ``id``, ``name`` ve ``label`` özelliklerinin değerlerine erişmeniz
gerekebilir. Örneğin ``id`` değerini almak için :

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.id }}

    .. code-block:: html+php

        <?php echo $form['task']->get('id') ?>

Form nesnesinin name niteliğindeki veriyi almak için de ``full_name``
değerini kullanmanız gerekir:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.full_name }}

    .. code-block:: html+php

        <?php echo $form['task']->get('full_name') ?>

Twig Şablon Fonksiyon Referansı
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Eğer Twig kullanıyorsanız tüm ekrana basma fonksiyonları hakkında bir 
:doc:`referans belgesi </reference/forms/twig_reference>` bulunmaktadır.
Bunu okuyarak şablon yardımcıları hakkındaki herşeyi ve her birisinin
kullandığı seçenekleri öğrenebilirsiniz.

.. index::
   single: Forms; Form sınıfları yaratmak

.. _book-form-creating-form-classes:

Form Sınıfları Yaratmak
------------------------

Şimdiye kadar gördüğünüz üzere bir form direkt olarak controller içerisinden
yaratılabiliyordu. Ancak bu konudaki en iyi uygulama ayrı, kendi başına çalışan
uygulamanın herhangi bir yerinden erişebileceğiniz bir PHP sınıfıdır.
Örneğimizdeki Görevler için yaratılacak formun mantığını barındıran bir sınıf
yaratalım:

.. code-block:: php

    // src/Acme/TaskBundle/Form/Type/TaskType.php

    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('widget' => 'single_text'));
        }

        public function getName()
        {
            return 'task';
        }
    }

Bu yeni sınıf, görev formunun yaratılmasında gereken tüm talimatları kapsar
(``getName()`` metodunun sadece bu form tipi için benzersiz bir ad döndürdüğüne
dikkat edin).
Bu artık controller içerisinde form nesnesi yaratırken kolaylıkla kullanılabilir:

.. code-block:: php

    // src/Acme/TaskBundle/Controller/DefaultController.php

    // add this new use statement at the top of the class
    use Acme\TaskBundle\Form\Type\TaskType;

    public function newAction()
    {
        $task = // ...
        $form = $this->createForm(new TaskType(), $task);

        // ...
    }

Form mantığının (logic) kendi sınıfının içerisinde olması, projeninizin
herhangibir yerinde bunu kolaylıkla yeniden kullanabileceğiniz anlamına
gelir. Bu formları yaratmanın en kolay yoludur fakat seçim size kalmış.

.. _book-forms-data-class:

.. sidebar:: ``data_class`` 'ını tanımlamak

    Her form nesnesi kendi içerisinde ilişkili olduğu veriyi tanımlayan sınıfın 
    ismine ihtiyaç duyar (Örn. ``Acme\TaskBundle\Entity\Task``). Genellikle
    bu sadece ``createForm`` metodunun ikinci argümanına bakılarak tahmin edilir
    (Örn : ``task``). Fakat sonra formları gömülü hale getirmeye başladığınızda bu işlem
    artık geçerliliğini yitirecektir.Her zaman gerekli olmamakla birlikte
    genel olarak açık bir şekilde form tipi sınıfına ``data_class`` seçeneğini
    kullanarak bu sınıfı tanıtmanız iyi bir fikirdir::

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            );
        }

.. tip::

    Formları objeler olarak tanımlarken tüm alanlar da tanımlanır. 
    Formun içerisindeki herhang bir alan nesne üzerinde eşleştirilemezse
    bir istisna üretilir (exception)

    Form'a ekstra alanlar ekleme durumunda (mesela "tüm koşulları kabul 
    ediyorum" işaret kutusu gibi) ilgili nesne içerisinde bu eksra alan
    tanımlanmayacaktır. Bunun için property_path seçeneğini ``false``
    yapmanız gereklidir::

        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('property_path' => false));
        }

    Ayrıca eğer form içerisine kullanıcı tarafından aktarılmayan 
    her veri gönderme esnasında ``null`` olarak kabul edilir.

.. index::
   pair: Forms; Doctrine

Formlar ve Doctrine
-------------------

Bir formun amacı nesneden (Örn. ``Task``) verileri çevirmek için HTML 
formu kullanmak ve kullanıcının girdiği verileri tekrar orijinal 
nesneye geri vermektir.
Bunun gibi ``Task`` nesnesinin veri tabanına yazılma konusu tamamen 
form konusu ile alakasıdır. Ancak eğer ``Task`` sınıfını Doctrine üzerinden 
veri tabanına yazılacak şekilde ayarlarsanız (Örn: bu form için
:ref:`eşleme verisi <book-doctrine-adding-mapping> eklerseniz)
form verisi gönderildiğinde veriler doğrulanırsa veritabanına yazılabilir::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }


Eğer bazı durumlarda orijinal ``$task`` nesnesine erişemiyorsanız bunu form
üzerinden çekebilirsiniz::

    $task = $form->getData();


Daha fazla bilgi için :doc:`Doctrine ORM kısmına</book/doctrine>` bakın.

Anlaşılması gereken anahtar konu form bağlı olduğunda kullanıcı tarafından
girilen verinin ilgili nesneye derhal aktarıldığıdır. Eğer bu veriyi
veri tabanına yazmak istiyorsanız basitçe bu nesnenin veritabanına
yazılması kendi nesnesi üzerinden gereklidir(gönderilen veriyi 
önceden üzerinde taşıyan nesne).

.. index::
   single: Forms; Gömülü formlar

Gömülü Formlar
--------------

Sıklıkla pek çok farklı nesneden oluşan alanlara sahip bir form yaratmak 
isteyeceksiniz. Örneğin bir kayıt formu ``User`` nesnesine ve ``Address``
nesnesine ait alanlardan oluşabilir. Çok şükür ki bu form bileşeni ile 
gayet basit ve kolay bir şekilde yapılır.

Tek Bir Nesne Gömmek
~~~~~~~~~~~~~~~~~~~~~~~~~

Varsayalım her ``Task`` basit bir ``Category`` nesnesine ait. Elbett
öncelikle ``Categoty`` nesnesini yaratarak başlıyoruz::

    // src/Acme/TaskBundle/Entity/Category.php
    namespace Acme\TaskBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Category
    {
        /**
         * @Assert\NotBlank()
         */
        public $name;
    }

Sonra ``Task`` sınıfına yeni bir ``category`` değişkeni ekliyoruz::

    // ...

    class Task
    {
        // ...

        /**
         * @Assert\Type(type="Acme\TaskBundle\Entity\Category")
         */
        protected $category;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setCategory(Category $category = null)
        {
            $this->category = $category;
        }
    }

Şimdi uygulamanız, kullanıcı tarafından değiştirilebilen bir ``Category``
form nesnesi yaratan bu yeni gerekiliklere göre güncellenmiştir.

    // src/Acme/TaskBundle/Form/Type/CategoryType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class CategoryType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('name');
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Category',
            );
        }

        public function getName()
        {
            return 'category';
        }
    }

Son işimiz ise ``Task`` nesnesinin ``Category`` 'sini task form'u içerisinden
düzenleyebilmek. Bunu yapabilmek için ``TaskType`` nesnesinin içerisine 
yeni ``CategoryType`` sınıfının özelliklerine sahip yeni bir ``category`` 
alanı eklemek gerekir::


.. code-block:: php

    public function buildForm(FormBuilder $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

``CategoryType`` taki alanlar şimdi ``TaskType`` sınıfı ekrana basılırken
bu alanlarda ekrana basılabilir. ``Category`` alanlarının ekrana basılması
aynı orijinal ``Task`` alanlarının ekrana basılması gibidir:

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}

        <h3>Category</h3>
        <div class="category">
            {{ form_row(form.category.name) }}
        </div>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- ... -->

        <h3>Category</h3>
        <div class="category">
            <?php echo $view['form']->row($form['category']['name']) ?>
        </div>

        <?php echo $view['form']->rest($form) ?>
        <!-- ... -->


Kullanıcı form verilerini gönderdiğinde ``Category`` alanları için gönderilen
form verisi ``Category`` nesnesi şeklinde düzenlenir ve sonra ``Task`` içerisindeki
``category`` alanına yüklenir.

``Category`` nesnesi şeklindeki veri doğal olarak ``$task->getCategory()``
metodu ile erişilebilir ve eğer kullanıcının ihtiyacı durumunda veri tabanına
yazılabilir.

Bir Form Kolleksiyonu Gömmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Eğer isterseniz bir form içerisinde bir formların kolleksiyonunu da gömebilirsiniz
(``Category`` formunun bir çok ``Product`` alt formundan oluştuğunu hayal edin).
Bunu ``collection`` alan tipini kullanarak yapabilirsiniz.

Daha fazla bilgi için ":doc:`/cookbook/form/form_collections`" tarif kitabı
girdisine ve :doc:`kolleksiyon</reference/forms/types/collection>` alan tipi
referansına bakın.

.. index::
   single: Forms; Tema uygulamak
   single: Forms; Alanları özelleştirmek

.. _form-theming:

Forma Tema Uygulamak
--------------------

Nasıl form ekrana basılırken her parçasını özelleştirebiliyorsanız, her
form "satırını" hataları gösteren işaretleri ya da hatta bir ``textarea``
etiketinin bile nasıl ekrana basılabileceğini özelleştirebilirsiniz. Bunun
herhangin bir limiti olmamakla beraber farklı yerlerde de bu özelleştirmeyi
kullanabilirsiniz.

Symfony ``label`` etiketleri, ``input`` etiketleri, hata mesajları ya da
herhangibir form parçası form parçası için bir şablon (template) kullanır.

Twig içerisinde her form "parçası" bir Twig bloğu halinde ifade edilit. Formun
bir parçasının  nasıl ekrana basılacağını düzenlemek isterseniz, sadece uygun
blokun koduna hükmetmeniz (override) gerekir.

PHP'de her form "parçası" ayrı bir şablon dosyası tarafından ekrana basılır.
Formun herhangi bir parçasının nasıl ekran basılacağını ayarlamak için sadece
var olan şablonun üzerine yeni şablonu bindirmeniz yeterlidir.

Bunun nasıl çalıştığını anlamak için ``form_row`` parçasını düzenleyerek 
her form satırını ``div`` elementleri arasında ve bu elemente bir css sınıfı
atayarak inceleyelim. Bunu yapmak için yeni bir şablon dosyarı yaratmamız
gerekir:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Form/fields.html.twig #}

        {% block field_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock field_row %}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Form/field_row.html.php -->

        <div class="form_row">
            <?php echo $view['form']->label($form, $label) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form, $parameters) ?>
        </div>

``filed_row`` form parçası pek çok form alanını ``form_row`` fonksiyonu
ile ekrana basar. Form bileşenine yeni ``field_row`` parçasının kullanılacağını
tanımlamak için, aşağıda gösterilen, formu ekrana basan şablonun en üstüne
şu satırı eklememiz gerekir:

.. configuration-block:: php

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' %}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' 'AcmeTaskBundle:Form:fields2.html.twig' %}

        <form ...>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form')) ?>

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form', 'AcmeTaskBundle:Form')) ?>

        <form ...>

``form_theme`` etiketi (Twig'de) verilen ve form ekrana basılırken kullanılacak
olan şablon dosyasını "içeriye aktarır" (import). Başka bir ifade ile ``form_row``
fonksiyonu şablon içerisinden çağırıldıktan sonra özelleştirilmiş tema şablonundaki
``field_row``  bloku kullanılacaktır(Symfony ile birlikte gelen varsayılan ``field_row``
yerine).

Özelleştirdiğiniz temanız tüm bloklara hükmetmez. Bir bloğun ekrana basılması esnasında
özelleştirilmiş temanız tarafından bu blok hükmedilmediyse tema motoru genel tema
içerisindeki özelliği kullanacaktır (bundle düzeyinde tanımlanan tema).

Eğer çeşitli özelleştirişmiş temalar verildiyse önce genel tema ayarlarına geçilmeden
önce sırasıyla araştırılacaktır.

Formun herhangi bir kısmını özelleştirmek için sadece uygun kısmı düzenlemeniz
(override) yeterlidir. Blok ya da dosya hükmetme konusu (override) sonraki
kısımın konusudur.

Daha geniş bir bilgi için :doc:`/cookbook/form/form_customization` belgesine bakın.

.. index::
   single: Forms; Şablon Parçalarının İsimlendirilmesi

.. _form-template-blocks:

Form Parçalarının İsimlendirilmesi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfonyde ekrana basılan formun her parçası - HTML form öğeleri, hatalar, etiketler, vs..-
Twig içinde blokların kolleksiyonundan oluşan temel bir tema içerisinde ve PHP'de de
şablon dosyaları kolleksiyonu içerisinde tanımlanır. 

Twig içerisinde her blok ayrı bir şablon dosyası (`form_div_layout.html.twig`_) 
olarak `Twig Bridge`_ içerisinde bulunur.
Bu dosyanın içerisinde her blokun bir form ve her varsayılan alan tipi'ni ekrana 
basaması gerektiğini görebilirsiniz.

PHP'de parçalar ayrı şablon dosyaları halindedir. Varsayılan olarak bu dosyalar 
framework bundle (`GitHub üzerinden görebilirsiniz`_) içerisindeki 
`Resources/views/Form` klasörü içerisinde bulunurlar.

Her parça ismi bir adlandırma şablonu içerisinde iki parça halinde alt tire (``_``)
sembolü ile ayrılır. Bazı örnekler:

* ``field_row`` -  ``form_row`` tarafından pek çok alanı ekrana basmak için kullanılır;
* ``textarea_widget`` - ``form_widget`` tarafından ``textarea`` alanını ekrana basmak için kullanılır;
* ``field_errors`` -  ``form_errors`` tarafından hatalı alanları ekrana basmak için kullanılır;

Her parça ``type_part`` şeklindeki basit bir şablona göre adlandırılır. ``type`` kısmı
alanı (Örn. ``textarea``, ``checkbox``, ``date``, vs..) temsil ederken, ``part`` 
kısmı ise *neyin* ekrana basılacağını belirler (Örn: ``label``, ``widget``, ``errors``, vs..).
Each fragment follows the same basic pattern: ``type_part``. The ``type`` portion
Varsayılan olarak 4 adet olası form *parçası* ekrana basılabilir:

+-------------+--------------------------+---------------------------------------------------------+
| ``label``   | (Örn: ``field_label``)   | Alanların Etiketlerini ekrana basar                     |
+-------------+--------------------------+---------------------------------------------------------+
| ``widget``  | (Örn: ``field_widget``)  | Alanın HTML kodunu ekrana basar                         |
+-------------+--------------------------+---------------------------------------------------------+
| ``errors``  | (Örn: ``field_errors``)  | Alanın Hatalarını ekrana basar                          |
+-------------+--------------------------+---------------------------------------------------------+
| ``row``     | (Örn: ``field_row``)     | alanın tüm satırını ekrana basar (label,widget& hatalar)|
+-------------+--------------------------+---------------------------------------------------------+

.. note::

    Aslında 3 adet *parça* daha bulunmasına rağmen - ``rows``, ``rest``, ve ``enctype`` -
    bunlar çok nadir şekilde hükmedilirler (override).

Alan tipini (Örn: ``textarea``) ve hangi *parçasının* özelleştirilebileceğini
(Örn: ``widget``) biliyor olmakla hükmetmeniz gereken(override)
(örn: ``textarea_widget``) parçanın da ismini bilebilirsiniz.

.. index::
   single: Forms; Şablon Parçası Kalıtımı

Şablon Parçası Kalıtımı
~~~~~~~~~~~~~~~~~~~~~~~

Bazı durumlarda, parçayı eksik olarak göstermek isteyebilirsiniz.
Örneğin Symfony tarafından sağlanan varsayılan temalarda ``textarea_errors``
kısmı bulunmaz. Peki bu alanlarda bir hata olduğunda hata mesajı nasıl çıkacak?

Cevap ``field_errors`` parçası üzerinden verilir. Symfony textarea tipindeki alanın
hatalarını ekrana basmadan önce ``textarea_errors`` 'a bakar bulamazsa ``field_
errors`` kısmına döner. Her alan tipinin bir *üst (parent)* tipi vardır 
(``textarea`` üst tipi ``field`` dır) ve Symfony tipin bu parçası yok ise
üst tipin ilgili parçasını kullanır.

Mesela *sadece* ``textarea`` alanlarının hatalarına hükmetmek için ``field_errors``
parçasını kopyalarak, ``textarea_errors`` adı ile değiştirip içerisini istediğiniz
gibi düzenleyin. *Tüm* alanların varsayılan hata mesajlarına hükmetmek (override) için
``field_errors` parçasının direkt alın ve kopyalayarak düzenleyin.

.. tip::

    Her alan tipinin "üst" tipi :doc:`form tip referans</reference/forms/types>`
    belgesinde tanımlanmıştır.

.. index::
   single: Forms; Genel Tema Uygulamak

Genel Form Teması Uygulamak
~~~~~~~~~~~~~~~~~~~~~~~~~~~
Yukarıdaki örnekte ``form_theme`` yardımcısını (Twig içerisinde) *sadece*
bu form için özel form parçalarını "içeri aktarmakta" (import) kullandınız.
Ayrıca Symfony2'ye tüm projenin formlarına düzenlemesini de söyleyebilirsiniz.

Twig
....

Otomatik olarak *tüm* şablonlar yaratılmadan önce ``fields.html.twig`` den 
türeyen bloku değiştirmek istiyorsanız uygulama konfigürasyon dosyanızı
şu şekilde değiştirmeniz gerekir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeTaskBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeTaskBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeTaskBundle:Form:fields.html.twig',
             ))
            // ...
        ));

``fields.html.twig`` şablonu içerisindeki herhangi bir blok şimdi genel
olarak tüm form'ların ekrana basılması esnasında tanımlanmıştır.

.. sidebar::  Tek bir Twig Dosyası ile Tüm Form Çıktılarını Özelleştirmek

    Twig'de ayrıca özelleştirme ihtiyacınıza göre form blogunu şablon içerisinde
    uygun bir yer de de özelleştirebilirsiniz:

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {# import "_self" as the form theme #}
        {% form_theme form _self %}

        {# make the form fragment customization #}
        {% block field_row %}
            {# custom field row output #}
        {% endblock field_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    ``{% form_theme form _self %}`` etiketi yapmak istediğiniz özelleştirmelere
    göre  form bloglarını direkt şablon içerisinden özelleştirecektir. 
    Bu metodu kullanarak tek bir şablon içerisinde form çıktısı özelleştirmelerini
    kolaylıkla yapabileceksiniz.

PHP
...

``Acme/TaskBundle/Resources/views/Form`` içerisinde otomatik olarak özelleştirilmiş
şablonları *tüm* şablonlar yaratılmadan önce almak (import) istiyorsanız 
uygulama konfigürasyon dosyanızı şu şekilde düzenleyin:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'AcmeTaskBundle:Form'
        # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeTaskBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeTaskBundle:Form',
             )))
            // ...
        ));


``Acme/TaskBundle/Resources/views/Form`` içerisindeki herhangi bir parça
artık form çıktılarına genel olarak kullanılabilir.

.. index::
   single: Forms; CSRF Koruması

.. _forms-csrf:

CSRF Koruması
---------------

CSRF - ya da `Cross-site request forgery`_ - zararlı kullanıcıların ,
zararsız kullanıcılar gibi verilerini bu kullanıcıların haberi olmadan
gönderilmesini sağlayan bir metoddur. Çok şükür ki bu ataklar form'larda
kullanılan CSRF biletleri (token) sayesinde önlenebilir.

Bu konudaki iyi haber, vasrsayılan olarak Symfony CSRF biletlerini otomatik
olarak formlara gömer ve doğruluğunu kontrol eder. Bunun anlamı CSRF Koruması
'nı hiç bir şey yapmadan otomatik olarak kullanabilirsimniz.  Aslında
bu kısımdaki her form CSRF koruması tarafından korunmaktadır!


CSRF koruması form içerisinde varsayılan olarak ``_token`` adıyla çağrılan
sadece siz ve kullanıcının bildiği bir veriden oluşan bir gizli alanın 
forma eklenmesi ile çalışır.
Bu kullanıcının forma verdiği -başka bir girdiyi değil- veriyi korur.
Symfony otomatik olarak bu biletin varlığını ve doğruluğunu kontrol eder.

``_token`` alanı eğer şablonunuz içerisinden ``form_rest()`` ile çağırırsanız
forma otomatik olarak eklenecek ve çıktıda gösterilmeyen tüm verileri içeren
gizli bir alandır (hidden field).


CSRF bileti form dan forma  kuralı ile özelleştirilebilir. Örneğin::

    class TaskType extends AbstractType
    {
        // ...

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class'      => 'Acme\TaskBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // a unique key to help generate the secret token
                'intention'       => 'task_item',
            );
        }

        // ...
    }


CSRF korumasını kapatmak için ``csrf_protection`` seçeneğini false
yapmanız gereklidir. Özelleştirmeler ayıca projeniz içerisinden 
genel olarak da yapılabilir. Bunun için 
:ref:`form konfigürasyon referansı <reference-framework-form>` 
kısmına bakın.

.. note::

    ``intention``  seçeneği seçimlik olmasına rağmen her form için
    yaratılan bilet değerini değişirtirir ve bu ciddi bir güvenlik artışı sağlar.

.. index:
   single: Forms; Sınıf olmadan 

Bir Formu Sınıf olmadan kullanmak
---------------------------------

Çoğu durumda bir form bir objeye bağlanır ve alanlar bu nesnenin değişkenleri
içerisinden değerleri yönetilir. Bunu açıkça bu bölümün `Task` sınıfında
görmüştünüz.

Fakat bazen formu bir sınıf olmadan kullanmak ve gönderilen
veriyi bir array (dize) içerisinden almak isteyebilirsiniz. Bu gerçekten
çok kolaydır::

    // Request namespace 'inin sınıfın üzerinde tanımlandığına emin olun
    use Symfony\Component\HttpFoundation\Request
    // ...

    public function contactAction(Request $request)
    {
        $defaultData = array('message' => 'Type your message here');
        $form = $this->createFormBuilder($defaultData)
            ->add('name', 'text')
            ->add('email', 'email')
            ->add('message', 'textarea')
            ->getForm();

            if ($request->getMethod() == 'POST') {
                $form->bindRequest($request);

                // data vei "name", "email", ve "message" anahtarları ile bir dize 
                // içerisindedir.
                $data = $form->getData();
            }

        // ... formu ekrana bas
    }

Varsayılan olarak, bir form verisinin gerçekte bir nesne yerine verilerin bir
dize ile çalışmak isteyebileceğinizi varsayar. Bu davranışı değiştirmek ve
ve bunu bir nesne içerisinde aktarmak için iki yolunuz bulunmaktadır:

1. Form yaratılırken bir objeye aktarmak (``createFormBuilder`` 'ın ilk argümanı
   ya da ``createForm`` 'un ikinci argümanında tanımlandığı gibi) 

2. form seçenekleri içerisindeki ``data_class`` özelliğinde bunu tanımlamak.

Eğer bunların hiç birisini istmiyorsanız bu durumda form değeri bir array
olarak dönecektir. Bu örnekte ``$defaultData`` bir nesne olmamasından dolayı 
(bir ``data_class`` seçeneği atanmamasından dolayı), ``$form->getData()`` 
nihayetinde bir array döndürecektir.

.. tip::

    Eğer isterseniz request nesnesinden POST değerlerini (bu durumda "name")
    direkt olarak şu şekilde çekebilirsiniz:

    .. code-block:: php

        $this->get('request')->request->get('name');

    Tavsiye olarak , getData() metodunun kullanımı pek çok durumda 
    veri form framework tarafından çevrildikten sonra döndüğünden 
    dolayı (genellikle bir nesne) en iyi yöntemdir.

Veri Doğrulama Eklemek
~~~~~~~~~~~~~~~~~~~~~~

Kalan tek parça veri doğrulamak. Genellikle ``$form->isValid()`` olarak
bilen metod, sınıfa uyguladığınız kısıtları okuyarak işletir. Ancak
formunuz bir sınıf değilse verilerin doğruluğunu nasıl belirleyeceksiniz?

Cevap, kendi kısıtlarınızı düzenlemek ve onları forma adapte etmektir.
Bu konudaki genel bir yaklaşım bir parça :ref:`veri doğrulama kısmında<book-validation-raw-values>`, 
işlenmiştir olmasına rağmen aşağıda kısa bir örnek verilmiştir::

    // controller sınıfının üzerinde namespace'leri aktar
    use Symfony\Component\Validator\Constraints\Email;
    use Symfony\Component\Validator\Constraints\MinLength;
    use Symfony\Component\Validator\Constraints\Collection;

    $collectionConstraint = new Collection(array(
        'name' => new MinLength(5),
        'email' => new Email(array('message' => 'Invalid email address')),
    ));

    // varsayılan değerleri olmadan bir form yarat ve kısıt seçeneklerini aktar
    $form = $this->createFormBuilder(null, array(
        'validation_constraint' => $collectionConstraint,
    ))->add('email', 'email')
        // ...
    ;

Şimdi `$form->bindRequest($request)` 'i çağırdığınızda kısıtlar yapılacak ve
form verisi üzerinde çalıştırılacaktır. Eğer bir form nesnesi kullanıyorsanız
``getDefaultOptions`` metodu belirli bir seçeneğe hükmeder (override)::

    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    use Symfony\Component\Validator\Constraints\Email;
    use Symfony\Component\Validator\Constraints\MinLength;
    use Symfony\Component\Validator\Constraints\Collection;

    class ContactType extends AbstractType
    {
        // ...

        public function getDefaultOptions(array $options)
        {
            $collectionConstraint = new Collection(array(
                'name' => new MinLength(5),
                'email' => new Email(array('message' => 'Invalid email address')),
            ));

            return array('validation_constraint' => $collectionConstraint);
        }
    }

Şimdi formu -veri doğrulama ile- form verisinin nesne yerine
bir array döndürülerek yaratma esnekliğine sahipsiniz.
Çoğu durumda bu bir nesneye form verisini bindirme  yöntemi 
en iyi -ve gerçekten sağlam- bir yöntemdir. 
Fakat basit formlarda bu mükemmel bir yaklaşımdır.


Son Sözler
-----------
Uygulamanızda karmaşık ve fonksiyonel formlar için blokların düzenlenmesinin
gerekliliğini tamamen biliyorsunuz. Formlar yapılırken formun ilk amacının bir nesneden
(``Task``) veriyi HTML formuna çevirdikten sonra kullanıcının veriyi düzenleyebilmesi olduğunu
aklınızdan çıkarmayın. Formun ikinci amacı ise kullanıcının göndermiş olduğu form 
verisini alıp yeninden nesnenin içerisine uygulamaktır.

Formların güçlü dünyası hakkında :doc:`Doctrine ile dosya yüklemeleri
</cookbook/doctrine/file_uploads>` ya da dinamik olarak istenilen sayıda
alt formlar eknenen bir form yaratmak (Örn: Javascript ile birden fazla
alanı bir yapılacaklar listesi formuna kullanıcı form verisini göndermeden
önce eklemek) gibi öğrenecek çok şey var. Bu konular için tarif kitabı b
konularına bakın. Ayrıca :doc:`alan tipleri referans belgesindeki</reference/forms/types>`
bu alan tiplerini ve seçeneklerinin nasıl kullanıldığı hakkındaki örneklere
de göz atmadan geçmeyin.

Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`File Field Reference </reference/forms/types/file>`
* :doc:`Creating Custom Field Types </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`
* :doc:`/cookbook/form/dynamic_form_generation`
* :doc:`/cookbook/form/data_transformers`

.. _`Symfony2 Form Bileşeni`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`GitHub üzerinden görebilirsiniz`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
