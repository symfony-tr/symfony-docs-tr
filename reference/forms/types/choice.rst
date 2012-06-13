.. index::
   single: Formlar; Alanlar; choice

choice Alan Tipi
=================

Birden fazla seçeneği olan alanlarda kullanıcının bir ya da daha fazla seçeneği
seçebileceği alanlar yaratr. Bu ``select`` etiketi ile yaratılabildiği gibi 
radio ya da checkbox olarak da yaratılabilir.

Bu alanı kullanmak için ``choice_list`` ya da ``choices`` seçeneklerini 
*tanımlamanız* gerekir.

+---------------------+---------------------------------------------------------------------------------+
| Ekranda Görüntüleme | çeşitli etiketlerde olabilir (aşağıya bakın)                                    |
+---------------------+---------------------------------------------------------------------------------+
| Seçenekler          | - `choices`_                                                                    |
|                     | - `choice_list`_                                                                |
|                     | - `multiple`_                                                                   |
|                     | - `expanded`_                                                                   |
|                     | - `preferred_choices`_                                                          |
|                     | - `empty_value`_                                                                |
|                     | - `empty_data`_                                                                 |
+---------------------+---------------------------------------------------------------------------------+
| Aktarılan           | - `required`_                                                                   |
| Seçenekler          | - `label`_                                                                      |
| (inherit)           | - `read_only`_                                                                  |
|                     | - `error_bubbling`_                                                             |
+---------------------+---------------------------------------------------------------------------------+
| Üst Tip             | :doc:`form</reference/forms/types/form>` (eğer genişletilirse), yoksa ``field`` |
+---------------------+---------------------------------------------------------------------------------+
| Sınıf               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`            |
+---------------------+---------------------------------------------------------------------------------+

Örnek Kullanım
--------------

Bu alanı kullanmanın en kolay yolu seçenekleri ``choices`` seçeneği altında
belirtmektir. Dize değişkenin anahtar(key) tarafı ilgili nesnenin alacağı
değer değer(value) (Örn. ``e``) tarafı ise bu değeri temsil eden metni 
(e.g. ``Erkek``) ifade eder.

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices'   => array('e' => 'Erkek', 'k' => 'Kadın'),
        'required'  => false,
    ));

``multiple`` seçeneğini true olarak belirlediğinizde kullanıcı çoklu seçim
yapabilir. Bunu oluşturan widget ekranda çoklu ``select`` etiketi ile oluşturulacak
ya da ``expanded`` seçeneğinin değerine göre bir dizi checkbox'tan oluşmuş 
bir liste halinde görüntülenecektir:

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Günaydın',
            'afternoon' => 'İyi Akşamlar',
            'evening'   => 'İyi Geceler',
        ),
        'multiple'  => true,
    ));

Ayrıca  ``choice_list`` seçeneğini kullanarak kullanılacak seçenekleri
bir nesneden de aktarabilirsiniz.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Alan Seçenekleri
----------------

choices
~~~~~~~

**tip**: ``array`` **varsayılan**: ``array()``

Bu alanı kullanmanız gerektiğinde seçenekleri belirleyebileceğiniz en temel
yol aşağıda verilmiştir. ``choices`` seçeneği, anahtar(key) değeri seçimin değeri, değer(value)
değeri ise seçimin metni şeklinde ayarlanan bir array(dize) değişkenidir::

    $builder->add('gender', 'choice', array(
        'choices' => array('e' => 'Erkek', 'k' => 'Kadın')
    ));

choice_list
~~~~~~~~~~~

**tip**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

Bu , bu alan için kullanılacak olan seçenekleri belirlemede kullanacağınız
bir yoldur. ``choice_list`` seçeneği mutlaka ``ChoiceListInterface`` 
interface'inin bir örneği (instance) olmalıdır. Daha özel durumlarda 
bu interface üzerinden türetilmiş bir sınıfı bu alanın değerlerini
belirlemekte kullanabilirsiniz.

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

Aktarılan seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
