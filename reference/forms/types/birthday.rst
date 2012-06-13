.. index::
   single: Formlar; Alanlar; birthday

birthday Alan Tipi
==================

Bir :doc:`date</reference/forms/types/date>` alanı doğum günü veri tipini
işlemekten sorumludur.

Bu tek bir metin kutusu olarak ya da üç alandan (ay, gün ve yıl) oluşan bir
seçim kutuları topluluğu olarak yaratılabilir.

Bu tip temel olarak  :doc:`date</reference/forms/types/date>` tipi ile aynıdır
ancak `years`_ alanı doğum günü tipine daha uygun olarak geliştirilmiştir. 
`years`_ seçeneği varsayılan olarak geçerli yıldan 120 yıl gerisine kadar gider.

+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Ana Veri Tipi        | ``DateTime``, ``string``, ``timestamp``, ya da  ``array`` olabilir. (bkz :ref:`input seçeneği <form-reference-date-input>`) |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Ekranda Görüntüleme  | `widget`_  ayarına bağlı olarak 3 select kutusu olabilir ya da  1 ya da 3 metin kutusu olabilir                             |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Seçenekler           | - `years`_                                                                                                                  |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Aktarılan            | - `widget`_                                                                                                                 |
| seçenekler           | - `input`_                                                                                                                  |
| (inherit)            | - `months`_                                                                                                                 |
|                      | - `days`_                                                                                                                   |
|                      | - `format`_                                                                                                                 |
|                      | - `pattern`_                                                                                                                |
|                      | - `data_timezone`_                                                                                                          |
|                      | - `user_timezone`_                                                                                                          |
|                      | - `invalid_message`_                                                                                                        |
|                      | - `invalid_message_parameters`_                                                                                             |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Üst Tip              | :doc:`date</reference/forms/types/date>`                                                                                    |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+
| Sınıf                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`                                                      |
+----------------------+-----------------------------------------------------------------------------------------------------------------------------+

Alan Seçenekleri
----------------

years
~~~~~

**tip**: ``array`` **varsayılan**: Geçerli yıldan 120 yıl gerisine kadar

Yıllar year alan tipinde listelenir. Bu seçenek sadece ``widget`` seçeneği
``choice`` olarak ayarlandığunda aktif olur.

Aktarılan seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`date</reference/forms/types/date>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/date_widget.rst.inc
    
.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc
    
.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Bu seçenekler :doc:`date</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

