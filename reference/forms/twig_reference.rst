.. index::
   single: Forms; Twig Form Fonksiyonları

Twig Şablonları Form Fonksiyonları
=====================================

Formları oluştururken kullanılabilecek olan tüm Twig fonksiyonları bu belgede verilmiştir.
Birkaç değişik fonksiyon bulunmaktadır ve her biri formun farklı kısımlarının (etiketler, 
hatalar, araçlar.. vs.) oluşturulmasından sorumludur.

form_label(form.name, label, variables)
---------------------------------------

Verilen alan için etiketi oluşturur. Özel olarak görünmesini istediğiniz bir etiketi
opsiyonel olan ikinci parametrede verebilirsiniz.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Aşağıdaki iki örnek de yapısal olarak doğrudur #}
    {{ form_label(form.name, 'Your Name', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Your name', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Verilan alan için hataları oluşturur.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# herhangi "genel" bir hatayı göster #}
    {{ form_errors(form) }}

form_widget(form.name, variables)
---------------------------------

Verilan alan için HTML araç çıktısını oluşturur. Eğer bütün bir forma ve ya form alanları topluluğuna
uygulanırsa, içerdiği her bir form satırı oluşturulur.

.. code-block:: jinja

    {# bir araç oluşturur ve "foo" sınıfını ekler #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

``form_widget`` için belirtilen ikinci parametre bir değişkenler dizisidir. 
En çok kullanılan değişken ``attr`` dir, HTML aracına uygulanacak HTML 
özelliklerini içerir. Bazı durumlarda, bazı tiplere ait şablonla ilgili 
başka özellikler de verilebilir. Bunlar her bir tip için ayrıca açıklanmıştır.

form_row(form.name, variables)
------------------------------

Verilen alan için form satırını oluşturur. Form satırları alanın etiketi, 
hataları ve HTML aracından oluşur.

.. code-block:: jinja

    {# bir alan için satırı oluştur ama etiket olarak "foo" göster #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

``form_row`` için verilen ikinci parametre bir değişkenler dizisidir. Symfony 
ile sağlanan şablonlarda üstteki örnek gibi sadece etiketin değiştirilmesine
izin verilmiştir.


form_rest(form, variables)
--------------------------

Verilen formda henüz oluşturulmamış tüm alanları oluşturur. Bu fonksiyonu 
formunuzda bir yerde mutlaka kullanmak iyi bir fikirdir, çünkü gizli ve ya
unuttuğunuz alanları oluşturacaktır.

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Eğer form en az bir tane dosya yükleme alanı içeriyorsa, bu fonksiyon 
gerekli ``enctype="multipart/form-data"`` form özelliğini oluşturur. 
Bu fonksiyonu da form içinde kullanmak iyi bir fikirdir:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>
