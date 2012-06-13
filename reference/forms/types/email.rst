.. index::
   single: Formlar; Alanlar; email

email Alan Tipi
================

``email`` alanı HTML5'in ``<input type="email" />`` etiketini ekranda 
gösteren bir metin alanıdır.

+--------------------+---------------------------------------------------------------------+
| Ekranda Görüntüleme| ``input`` ``email`` alanı (ya da metin kutusu)                      |
+--------------------+---------------------------------------------------------------------+
| Aktarılan          | - `max_length`_                                                     |
| seçenekler         | - `required`_                                                       |
|                    | - `label`_                                                          |
|                    | - `trim`_                                                           |
|                    | - `read_only`_                                                      |
|                    | - `error_bubbling`_                                                 |
+--------------------+---------------------------------------------------------------------+
| Üst Tip            | :doc:`field</reference/forms/types/field>`                          |
+--------------------+---------------------------------------------------------------------+
| Class              | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\EmailType` |
+--------------------+---------------------------------------------------------------------+

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler  :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
