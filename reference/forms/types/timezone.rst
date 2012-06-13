.. index::
   single: Formlar; Alanlar; timezone

timezone Alan Tipi
==================

``timezone`` tipi ``ChoiceType`` tipinin bir alt tipi olarak kullanıcının
mevcut tüm zaman kuşakları seçenekleri arasından seçim yapmasını sağlar.

Her zaman kuşağının "değeri" ``America/Chicago`` ya da ``Europe/Istanbul`` 
gibi tam açık bir değerdir.

``choice`` tipinde ``choices`` ya da ``choice_list`` seçenekleri belirlenmese
dahi bu alan kullanıcının yerel bilgisine göre geniş bir zaman kuşağı seçeneği
listesi ile doldurulacaktur. Bu seçenekleri kendiniz düzenlemek istiyorsanız
sadece ``choice`` seçeneğine istediğiniz zaman kuşaklarını manuel olarak
girebilirsiniz.

+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | çeşitli şekillerde (bkz :ref:`forms-reference-choice-tags`)            |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `multiple`_                                                          |
| seçenekler           | - `expanded`_                                                          |
| (inherit)            | - `preferred_choices`_                                                 |
|                      | - `empty_value`_                                                       |
|                      | - `error_bubbling`_                                                    |
|                      | - `required`_                                                          |
|                      | - `label`_                                                             |
|                      | - `read_only`_                                                         |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`choice</reference/forms/types/choice>`                           |
+----------------------+------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+----------------------+------------------------------------------------------------------------+

Aktarılan Seçenekler
---------------------

Bu seçenekler :doc:`choice</reference/forms/types/choice>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Bu seçenekler :doc:`field</reference/forms/types/field>`  tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
