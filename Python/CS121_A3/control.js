function postData(input){
    $.post(
        "http://127.0.0.1:5000/search",
        {'query': input},
        function(response){
            var table = $('table');
            table.children().remove();
            count = 1;
            if (response[1].length != 0) {
                var row =  "<tr><td>" + "Search took " + response[0] + " seconds"+ "</td></tr>"
                table.append(row);}
            for(let url of response[1]){
                if (count < 101){
                    var row =  "<tr><td>" + count + " url: " + ('<a href="'+url+'" target="_blank">'+url+'</a>') +  "</td></tr>"
                    count++;
                    table.append(row);
                }
            }
            
        },
    );
}


document.addEventListener('DOMContentLoaded', ()=>{
    let searchInput = document.getElementById("search");
    document.getElementById("searchB").addEventListener('click', ()=>{
        let query=searchInput.value;
        postData(query);
    })
})
