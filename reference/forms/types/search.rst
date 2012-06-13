.. index::
   single: Formlar; Alanlar; search

search Alan Tipi
================

Bu bazı tarayıcılar tarafından desteklenen  ``<input type="search" />`` 
tipinde bir metin kutusu ekrana basar.

Daha fazla bilgi için `DiveIntoHTML5.info`_ sitesine bakın.

+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | ``input search`` alanı                                                 |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `max_length`_                                                        |
| seçenekler           | - `required`_                                                          |
| (inherit)            | - `label`_                                                             |
|                      | - `trim`_                                                              |
|                      | - `read_only`_                                                         |
|                      | - `error_bubbling`_                                                    |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`text</reference/forms/types/text>`                               |
+----------------------+------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SearchType`   |
+----------------------+------------------------------------------------------------------------+

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler  :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. _`DiveIntoHTML5.info`: http://diveintohtml5.info/forms.html#type-search
