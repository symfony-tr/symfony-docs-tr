.. index::
   single: Formlar; Alanlar; country

country Alan Tipi
==================

``country`` tipi ``ChoiceType`` tipinin dünya üzerindeki ülkeleri gösteren
bir alt tipidir. Bir bonus olarak eklenmiştir ve kullanıcının kendi dilinde
ülkeleri gösterir.

Her ülkenin "value" değeri ülkelerin iki harfli kodlarıdır.

.. note::

   Kullanıcının yerel bilgisini tahmn etmek için `Locale::getDefault()`_ kullanılır.

``choice`` tipinde ``choices`` ya da ``choice_list`` seçeneklerini belirlemezseniz
bile bu alan tipi, dünya üzerindeki ülkelerin listesini otomatik olarak oluşturacaktır.
Bu bu ülke listesini kendinize göre ayarlamak için sadece ``choice`` seçeneğine kendinize
göre düzenlemeniz yeterlidir.

+--------------------+-------------------------------------------------------------------------+
| Ekranda Görüntüleme| çeşitli şekilde olabilir. (bkz:ref:`forms-reference-choice-tags`)       |
+--------------------+-------------------------------------------------------------------------+
| Aktarılan          | - `multiple`_                                                           |
| Seçenekler         | - `expanded`_                                                           |
| (inherit)          | - `preferred_choices`_                                                  |
|                    | - `empty_value`_                                                        |
|                    | - `error_bubbling`_                                                     |
|                    | - `required`_                                                           |
|                    | - `label`_                                                              |
|                    | - `read_only`_                                                          |
+--------------------+-------------------------------------------------------------------------+
| Üst Tip            | :doc:`choice</reference/forms/types/choice>`                            |
+--------------------+-------------------------------------------------------------------------+
| Class              | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CountryType`   |
+--------------------+-------------------------------------------------------------------------+

Aktarılan seçenekler (inherit)
------------------------------

Bu seçenekler  :doc:`choice</reference/forms/types/choice>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Bu seçenekler  :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php
