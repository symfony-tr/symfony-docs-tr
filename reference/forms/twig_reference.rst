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

Verilan alanýn Renders the HTML widget of a given field. If you apply this to an entire form
or collection of fields, each underlying form row will be rendered.

.. code-block:: jinja

    {# render a widget, but add a "foo" class to it #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

The second argument to ``form_widget`` is an array of variables. The most
common variable is ``attr``, which is an array of HTML attributes to apply
to the HTML widget. In some cases, certain types also have other template-related
options that can be passed. These are discussed on a type-by-type basis.

form_row(form.name, variables)
------------------------------

Renders the "row" of a given field, which is the combination of the field's
label, errors and widget.

.. code-block:: jinja

    {# render a field row, but display a label with text "foo" #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

The second argument to ``form_row`` is an array of variables. The templates
provided in Symfony only allow to override the label as shown in the example
above.

form_rest(form, variables)
--------------------------

This renders all fields that have not yet been rendered for the given form.
It's a good idea to always have this somewhere inside your form as it'll
render hidden fields for you and make any fields you forgot to render more
obvious (since it'll render the field for you).

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

If the form contains at least one file upload field, this will render the
required ``enctype="multipart/form-data"`` form attribute. It's always a
good idea to include this in your form tag:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>