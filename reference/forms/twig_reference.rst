.. index::
   single: Forms; Twig form function reference

Twig Þablonlarý Form Fonksiyonlarý
=====================================

Formlarý oluþtururken kullanýlabilecek olan tüm Twig fonksiyonlarý bu belgede verilmiþtir.
Birkaç deðiþik fonksiyon bulunmaktadýr ve her biri formun farklý kýsýmlarýnýn (etiketler, 
hatalar, araçlar.. vs.) oluþturulmasýndan sorumludur.

form_label(form.name, label, variables)
---------------------------------------

Verilen alan için etiketi oluþturur. Özel olarak görünmesini istediðiniz bir etiketi
opsiyonel olan ikinci parametrede verebilirsiniz.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Aþaðýdaki iki örnek de yapýsal olarak doðrudur #}
    {{ form_label(form.name, 'Your Name', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Your name', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Verilan alan için hatalarý oluþturur.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# herhangi "genel" bir hatayý göster #}
    {{ form_errors(form) }}

form_widget(form.name, variables)
---------------------------------

Verilan alan için HTML araç çýktýsýný oluþturur. Eðer bütün bir forma ve ya form alanlarý topluluðuna
uygulanýrsa, içerdiði her bir form satýrý oluþturulur.

.. code-block:: jinja

    {# bir araç oluþturur ve "foo" sýnýfýný ekler #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

``form_widget`` için belirtilen ikinci parametre bir deðiþkenler dizisidir. 
En çok kullanýlan deðiþken ``attr`` dir, HTML aracýna uygulanacak HTML 
özelliklerini içerir. Bazý durumlarda, bazý tiplere ait þablonla ilgili 
baþka özellikler de verilebilir. Bunlar her bir tip için ayrýca açýklanmýþtýr.

form_row(form.name, variables)
------------------------------

Verilen alan için form satýrýný oluþturur. Form satýrlarý alanýn etiketi, 
hatalarý ve HTML aracýndan oluþur.

.. code-block:: jinja

    {# bir alan için satýrý oluþtur ama etiket olarak "foo" göster #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

``form_row`` için verilen ikinci parametre bir deðiþkenler dizisidir. Symfony 
ile saðlanan þablonlarda üstteki örnek gibi sadece etiketin deðiþtirilmesine
izin verilmiþtir.


form_rest(form, variables)
--------------------------

Verilen formda henüz oluþturulmamýþ tüm alanlarý oluþturur. Bu fonksiyonu 
formunuzda bir yerde mutlaka kullanmak iyi bir fikirdir, çünkü gizli ve ya
unuttuðunuz alanlarý oluþturacaktýr.

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Eðer form en az bir tane dosya yükleme alaný içeriyorsa, bu fonksiyon 
gerekli ``enctype="multipart/form-data"`` form özelliðini oluþturur. 
Bu fonksiyonu da form içinde kullanmak iyi bir fikirdir:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>