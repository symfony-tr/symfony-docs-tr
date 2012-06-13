.. index::
   single: Formlar; Alanlar; url

url Alan Tipi
=============

``url`` alanı değerin başına, eğer kullanıcı tarafından girilmediyse, 
otomatik olarak verilen protokol ismini ekleyen bir metin alanıdır (örn: ``http://``) 

+---------------------+-------------------------------------------------------------------+
|Ekranda Görüntüleme  | ``input url`` alanı                                               |
+---------------------+-------------------------------------------------------------------+
| Seçenekler          | - `default_protocol`_                                             |
+---------------------+-------------------------------------------------------------------+
| Aktarılan           | - `max_length`_                                                   |
| seçenekler          | - `required`_                                                     |
| (inherit)           | - `label`_                                                        |
|                     | - `trim`_                                                         |
|                     | - `read_only`_                                                    |
|                     | - `error_bubbling`_                                               |
+---------------------+-------------------------------------------------------------------+
| Üst Sınıf           | :doc:`text</reference/forms/types/text>`                          |
+---------------------+-------------------------------------------------------------------+
| Sınıf               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+---------------------+-------------------------------------------------------------------+

Alan Seçenekleri
----------------

default_protocol
~~~~~~~~~~~~~~~~

**tip**: ``string`` **varsayılan**: ``http``

Eğer kullanıcı tarafından girilen değerin başında bir protokol ismi 
(örn: http:// ya da ftp:// gibi) yoksa bu değerde belirtilen protokol
ismi değerin başına otomatik olarak eklenip forma gönderilir.

Aktarılan Seçenekler (inherit)
------------------------------

Bu seçenekler :doc:`field</reference/forms/types/field>` tipinden aktarılmıştır:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
