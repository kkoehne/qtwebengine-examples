var qt = { 'jQuery': jQuery.noConflict(true) };
qt.jQuery('img').each(
    function () {
        qt.jQuery(this).css('-webkit-transition', '-webkit-transform 2s');
        qt.jQuery(this).css('-webkit-transform', 'rotate(180deg)');
    }
);
