.. index::
   single: Formlar; Alanlar; text

text Alan Tipi
===============

En basit metin kutusunu temsil eder.

+----------------------+--------------------------------------------------------------------+
| Ekranda Görüntüleme  | ``input`` ``text`` alanı                                           |
+----------------------+--------------------------------------------------------------------+
| Aktarılan            | - `max_length`_                                                    |
| seçenekler           | - `required`_                                                      |
| (inherit)            | - `label`_                                                         |
|                      | - `trim`_                                                          |
|                      | - `read_only`_                                                     |
|                      | - `error_bubbling`_                                                |
+----------------------+--------------------------------------------------------------------+
| Üst Tip              | :doc:`field</reference/forms/types/field>`                         |
+----------------------+--------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextType` |
+----------------------+--------------------------------------------------------------------+


Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
