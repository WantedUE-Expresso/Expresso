from flask import Flask, jsonify, request, render_template_string

app = Flask(__name__)

# In-memory database
Users = [
    {
        "name": "김전사",
        "age": 25,
        "item": ["long sword", "iron shield", "red potion"]
    },
    {
        "name": "이마법",
        "age": 32,
        "item": ["magic wand", "blue robe", "mana potion"]
    },
    {
        "name": "박궁수",
        "age": 28,
        "item": ["short bow", "leather armor", "arrows"]
    }
]

HTML_FORM = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Post Test Page</title>
    <style>
        body { font-family: sans-serif; margin: 2em; }
        h1, h2 { color: #333; }
        form { background-color: #f9f9f9; padding: 1.5em; border-radius: 8px; border: 1px solid #ddd; }
        input[type="text"], input[type="number"] { width: 300px; padding: 8px; margin-bottom: 10px; border-radius: 4px; border: 1px solid #ccc; }
        input[type="submit"] { background-color: #4CAF50; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }
        input[type="submit"]:hover { background-color: #45a049; }
    </style>
</head>
<body>
    <h1>Unreal Engine POST Test Form</h1>
    <p>이 페이지는 Unreal 클라이언트의 HTTP 요청을 테스트하기 위한 HTML 폼입니다.</p>
    <p>데이터는 JSON API 엔드포인트인 <code>/</code>로 전송됩니다.</p>
    
    <h2>새 사용자 추가 (POST to /)</h2>
    <form action="/" method="post" enctype="application/x-www-form-urlencoded">
        <label for="name">Name:</label><br>
        <input type="text" id="name" name="name" value="최도적"><br><br>
        
        <label for="age">Age:</label><br>
        <input type="number" id="age" name="age" value="22"><br><br>
        
        <label for="item">Items (comma-separated):</label><br>
        <input type="text" id="item" name="item" value="dagger,leather gloves,smoke bomb"><br><br>
        
        <input type="submit" value="Submit">
    </form>
</body>
</html>
'''

@app.route('/', methods=['GET', 'POST'])
def handle_api():
    if request.method == 'POST':
        content_type = request.content_type
        if 'application/json' in content_type:
            new_player = request.get_json()
        else: # Handle form data for testing
            new_player = {
                "name": request.form.get('name'),
                "age": int(request.form.get('age')),
                "item": [item.strip() for item in request.form.get('item', '').split(',')]
            }
        
        if not new_player or not new_player.get('name'):
            return jsonify({"error": "Invalid data"}), 400

        print(f"Received POST data: {new_player}")
        Users.append(new_player)
        return jsonify(new_player), 201
    
    # On GET request, handle optional name search
    search_name = request.args.get('name')
    if search_name:
        for user in Users:
            if user['name'] == search_name:
                return jsonify([user])
        return jsonify({"error": "User not found"}), 404
    else:
        return jsonify(Users)

@app.route('/form')
def show_test_form():
    """Serves the HTML form for manual testing."""
    return render_template_string(HTML_FORM)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=4000, debug=True)
