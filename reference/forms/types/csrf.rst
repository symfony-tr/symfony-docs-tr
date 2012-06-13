.. index::
   single: Formlar; Alanlar; csrf

csrf Alan Tipi
===============

``csrf`` tipi CSRF token'ını içeren hizli bir input alanı yaratır.

+--------------------+--------------------------------------------------------------------+
| Ekranda Görüntüleme| ``input`` ``hidden`` alanı                                         |
+--------------------+--------------------------------------------------------------------+
| Seçenekler         | - ``csrf_provider``                                                |
|                    | - ``intention``                                                    |
|                    | - ``property_path``                                                |
+--------------------+--------------------------------------------------------------------+
| Üst Tip            | ``hidden``                                                         |
+--------------------+--------------------------------------------------------------------+
| Sınıf              | :class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\CsrfType` |
+--------------------+--------------------------------------------------------------------+

Alan Seçenekleri
----------------

csrf_provider
~~~~~~~~~~~~~

**tip**: ``Symfony\Component\Form\CsrfProvider\CsrfProviderInterface``

``CsrfProviderInterface`` 'i CSRF token'ını yaratan nesnedir. Eğer
belirtilmezse bu varsayılan sağlayıcı olarak belirlenir.

intention
~~~~~~~~~

**tip**: ``string``

CSRF token'ini yaratırken isteğe bağlı olarak kullanılan benzersiz
bir belirteçtir.

.. include:: /reference/forms/types/options/property_path.rst.inc
