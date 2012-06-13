.. index::
   single: Formlar; Alanlar;  language

language Alan Tipi
===================

``language`` tipi ``ChoiceType`` tipinin bir alt tipi olarak kullanıcıya
geniş bir dil seçeneği sunan bir seçim listesidir. Bonus olarak eklenen bu alan
kullanıcının kendi dilinde lisan adlarını sıralar.

Her dilin değeri  *Unicode dil tanımlayıcısı* olarak tanımlanır.
(Örn : ``tr`` ya da ``zh-Hant``).

.. note::

   Kullanıcının yerel bilgisinin tahmin edilmesinde `Locale::getDefault()`_
   kullanılır.
   

``choice`` tipinde  ``choices`` ya da ``choices_list`` seçeneğini belirlemeseniz
dahi tüm Lisan adları bu alanda otomatik olarak listlenecektir. Eğer isterseniz
size gereken diller için ``choice`` seçeneğini kullanarak bu listeyi özelleştirebilirsiniz.


+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | çeşitli şekilde olabilir (bkz :ref:`forms-reference-choice-tags`)      |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `multiple`_                                                          |
| Seçenekler           | - `expanded`_                                                          |
| (inherit)            | - `preferred_choices`_                                                 |
|                      | - `empty_value`_                                                       |
|                      | - `error_bubbling`_                                                    |
|                      | - `required`_                                                          |
|                      | - `label`_                                                             |
|                      | - `read_only`_                                                         |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`choice</reference/forms/types/choice>`                           |
+----------------------+------------------------------------------------------------------------+
| Class                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+----------------------+------------------------------------------------------------------------+

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`choice</reference/forms/types/choice>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php
