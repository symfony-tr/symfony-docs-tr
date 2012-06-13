.. index::
   single: Formlar; Alanlar; checkbox

checkbox Alan Tipi
===================

Tek bir işaret kutusu (checkbox) yaratır. Bu daima Mantıksal bir değer (boolean)
için kullanılmalıdır. Eğer kutu işaretlenirse bu alan true değerini alır, işaretli
değilse bu değer false olacaktır.

+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | ``input`` ``text`` alanı                                               |
+----------------------+------------------------------------------------------------------------+
| Seçenekler           | - `value`_                                                             |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `required`_                                                          |
| Seçenekler           | - `label`_                                                             |
| (inherit)            | - `read_only`_                                                         |
|                      | - `error_bubbling`_                                                    |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`field</reference/forms/types/field>`                             |
+----------------------+------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+----------------------+------------------------------------------------------------------------+

Örnek Kullanım
---------------

.. code-block:: php

    $builder->add('public', 'checkbox', array(
        'label'     => 'Bu girdi herkeze görünsün mü?',
        'required'  => false,
    ));

Alan Seçenekleri
----------------

value
~~~~~

**tip**: ``mixed`` **varsayılan**: ``1``

value işaret kutusunun gerçekte aldığı değeri temsil eder. Bunu nesneniz
içerisinde ayarlamanız herhangi bir etkiye sebep olmaz.

Aktarılan seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
