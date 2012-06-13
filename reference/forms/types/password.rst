.. index::
   single: Formlar; Alanlar; password

password Alan Tipi
==================

``password`` alanı bir parola girilen metin kutusunu ekranda gösterir.
+----------------------+------------------------------------------------------------------------+
| Ekranda Görüntüleme  | ``input`` ``password`` alanı                                           |
+----------------------+------------------------------------------------------------------------+
| Seçenekler           | - `always_empty`_                                                      |
+----------------------+------------------------------------------------------------------------+
| Aktarılan            | - `max_length`_                                                        |
| Seçenekler           | - `required`_                                                          |
| (inherit)            | - `label`_                                                             |
|                      | - `trim`_                                                              |
|                      | - `read_only`_                                                         |
|                      | - `error_bubbling`_                                                    |
+----------------------+------------------------------------------------------------------------+
| Üst Tip              | :doc:`text</reference/forms/types/text>`                               |
+----------------------+------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PasswordType` |
+----------------------+------------------------------------------------------------------------+

Alan Seçenekleri
-----------------

always_empty
~~~~~~~~~~~~

**tip**: ``Boolean`` **varsayılan**: ``true``

Eğer true yapılırsa alan içinde bir değer olsa bile *herzaman* boş olarak 
ekranda gösterilecektir. False olarak ayarlandığında password alanı ``value``
değerinin içerisinde gerçek parola değeri olacak şekilde ekrana basılacaktır. 

Eğer bazı nedenlerden parola alanını ekranda *parola* değeri ile birlikte
ekranda göstermek istiyorsanız bu seçeneğin değerini false yapın.

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
