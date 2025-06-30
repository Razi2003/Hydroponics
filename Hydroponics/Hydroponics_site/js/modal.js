$(document).on("click", ".clickLink", function () {
    var fileName = $(this).data('id');

    path = "pdfs//"+fileName+"#toolbar=0";  // To Hide Toolbar
    var src = $('#myframe').attr('src'); ;

    $(".modal-body #filename").text(fileName);   //sets filename in modal 
    $('.modal-body #myframe').attr('src', path);   //sets src value in  in modal iframe

    });