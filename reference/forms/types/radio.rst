.. index::
   single: Formlar; Alanlar;  radio

radio Alan Tipi
================

Her zaman boolean değer içeren bir radio butonu yaratır. Eğer radio seçili
ise bu alanın değeri true olacak aksi durumda ise false olacaktır.

``radio`` tipi genellikle tek başına direkt olarak kullanılmaz. Genellikle 
:doc:`choice</reference/forms/types/choice>` gibi diğer tipler tarafından 
içsel olarak kullanılır.

Eğer bir mantıksal alana ihtiyacınız var ise  :doc:`checkbox</reference/forms/types/checkbox>`
alanını kullanın.

+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | ``input`` ``radio`` Alanı                                              |
+----------------------+------------------------------------------------------------------------+
| Seçenekler           | - `value`_                                                             |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `required`_                                                          |
| seçenekler           | - `label`_                                                             |
| (inherit)            | - `read_only`_                                                         |
|                      | - `error_bubbling`_                                                    |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`field</reference/forms/types/field>`                             |
+----------------------+------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType`    |
+----------------------+------------------------------------------------------------------------+

Alan Seçenekleri
-----------------

value
~~~~~

**tip**: ``mixed`` **değer**: ``1``

value alanının aldığı değer radio butonunun gerçekte aldığuı değerdir. Bu
değeri nesneniz içerisinden ayarlamanız herhangi bir etkiye sebep olmaz.

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler  the :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
