use proc_macro::TokenStream;
use quote::quote;
use syn::{
    parse_macro_input, parse_quote, Expr,FnArg, ItemFn, Stmt,
    visit_mut::{VisitMut}, DeriveInput, Fields, Data, Block
};

#[proc_macro_derive(ToPos)]
pub fn derive_to_pos(input: TokenStream) -> TokenStream
{
    let input = parse_macro_input!(input as DeriveInput);
    let name = &input.ident;

    match &input.data
    {
        Data::Struct(data_struct) =>
        {
            match &data_struct.fields
            {
                Fields::Named(fields) =>
                {
                    let has_x = fields.named.iter().any(|f|
					{
                        f.ident.as_ref().map_or(false, |i| i == "x")
                    });
                    let has_y = fields.named.iter().any(|f|
					{
                        f.ident.as_ref().map_or(false, |i| i == "y")
                    });
                    
                    if !has_x || !has_y
                    {
                        return syn::Error::new_spanned(
                            &input.ident,
                            "ToPos can only be derived for structs with 'x' and 'y' fields"
                        ).to_compile_error().into();
                    }
                }
                _ =>
                {
                    return syn::Error::new_spanned(
                        &input.ident,
                        "ToPos can only be derived for structs with named fields"
                    ).to_compile_error().into();
                }
            }
        }
        _ =>
        {
            return syn::Error::new_spanned(
                &input.ident,
                "ToPos can only be derived for structs"
            ).to_compile_error().into();
        }
    }

    let expanded = quote!
    {
        impl ::aoc::map::ToPos for #name
        {
            fn to_pos(&self) -> ::aoc::map::Pos
            {
                ::aoc::map::Pos
                {
                    y: self.y,
                    x: self.x,
                }
            }
        }
    };

    TokenStream::from(expanded)
}


/// This macro does nothing - it's just a marker for the add_show macro
#[proc_macro]
pub fn insert_here(_input: TokenStream) -> TokenStream
{
    TokenStream::new()
}


/// A procedural macro that duplicates a function with modifications:
/// - Creates a new function with "_show" suffix
/// - Adds a new parameter: map: &[T] where T: AsRef<[char]>
/// - Replaces all insert_here!(code) with the code inside the macro call
#[proc_macro_attribute]
pub fn add_show(_args: TokenStream, input: TokenStream) -> TokenStream
{
    let input_fn = parse_macro_input!(input as ItemFn);
    let mut original_fn = input_fn.clone();
    
    // Clean up the original function by removing insert_here! calls
    let mut original_cleaner = InsertHereRemover;
    original_cleaner.visit_item_fn_mut(&mut original_fn);
    
    // Create the modified function
    let mut modified_fn = input_fn;
    
    // Change the function name by adding "_show"
    let original_name = &modified_fn.sig.ident;
    let new_name = syn::Ident::new(
        &format!("{}_show", original_name),
        original_name.span(),
    );
    modified_fn.sig.ident = new_name;
    
    // Check if 'T' generic parameter already exists
    let has_t_generic = modified_fn.sig.generics.params.iter().any(|param|
	{
        if let syn::GenericParam::Type(type_param) = param
		{
            type_param.ident == "T"
        } else {
            false
        }
    });
    
    // Add the new generic parameter T only if it doesn't exist
    if !has_t_generic
	{
        let generic_param: syn::GenericParam = parse_quote! { T };
        modified_fn.sig.generics.params.push(generic_param);
        
        // Add the where clause for T: AsRef<[char]>
        let where_predicate: syn::WherePredicate = parse_quote! { T: AsRef<[char]> };
        if modified_fn.sig.generics.where_clause.is_none()
		{
            modified_fn.sig.generics.where_clause = Some(parse_quote! { where });
        }
        let where_clause = modified_fn.sig.generics.where_clause.as_mut().unwrap();
        where_clause.predicates.push(where_predicate);
    }
    
    // Check if 'map' parameter already exists
    let has_map_param = modified_fn.sig.inputs.iter().any(|arg|
	{
        if let FnArg::Typed(pat_type) = arg
		{
            if let syn::Pat::Ident(pat_ident) = &*pat_type.pat
			{
                pat_ident.ident == "map"
            } else {
                false
            }
        } else {
            false
        }
    });
    
    // Add the new parameter: map: &[T] only if it doesn't exist
    if !has_map_param
	{
        let new_param: FnArg = parse_quote! { map: &[T] };
        modified_fn.sig.inputs.push(new_param);
    }
    
    // Replace insert_here! macro calls with their contents in the _show version
    let mut replacer = InsertHereReplacer;
    replacer.visit_item_fn_mut(&mut modified_fn);
    
    // Generate the output containing both functions
    let expanded = quote!
	{
        #original_fn
        #modified_fn
    };
    
    // Uncomment for debugging:
    // eprintln!("Generated code:\n{}", expanded);
    
    TokenStream::from(expanded)
}

/// Visitor to remove insert_here! macro calls entirely
struct InsertHereRemover;

impl VisitMut for InsertHereRemover
{
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt)
	{
        match stmt
		{
            Stmt::Expr(expr, _semi_token) =>
			{
                if let Expr::Macro(macro_expr) = expr
				{
                    if macro_expr.mac.path.is_ident("insert_here")
					{
                        // Replace with an empty block expression
                        *expr = parse_quote! { {} };
                        return;
                    }
                }
                self.visit_expr_mut(expr);
            }
            Stmt::Macro(macro_stmt) =>
			{
                if macro_stmt.mac.path.is_ident("insert_here")
				{
                    // Replace the entire statement with an empty statement
                    *stmt = parse_quote! { {} };
                    return;
                }
                syn::visit_mut::visit_stmt_mut(self, stmt);
            }
            _ =>
			{
                syn::visit_mut::visit_stmt_mut(self, stmt);
            }
        }
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr)
	{
        if let Expr::Macro(macro_expr) = expr
		{
            if macro_expr.mac.path.is_ident("insert_here")
			{
                // Replace with unit expression
                *expr = parse_quote! { () };
                return;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

/// Visitor to replace insert_here!(code) macro calls with the code inside them
struct InsertHereReplacer;

impl VisitMut for InsertHereReplacer
{
    fn visit_block_mut(&mut self, block: &mut Block)
	{
        // Process each statement in the block
        for stmt in &mut block.stmts
		{
            self.visit_stmt_mut(stmt);
        }
    }

    fn visit_stmt_mut(&mut self, stmt: &mut Stmt)
	{
        //eprintln!("Visiting statement type");
        
        match stmt
		{
            Stmt::Expr(expr, _semi_token) =>
			{
                //eprintln!("Found Stmt::Expr, has_semi: {}", semi_token.is_some());
                if let Expr::Macro(macro_expr) = expr
				{
                    //eprintln!("Found macro in Stmt::Expr: {}", macro_expr.mac.path.get_ident().map_or("none".to_string(), |i| i.to_string()));
                    if macro_expr.mac.path.is_ident("insert_here")
					{
                        //eprintln!("Found insert_here! in Stmt::Expr context");
                        let tokens = &macro_expr.mac.tokens;
                        //eprintln!("Tokens: {}", tokens);
                        
                        // Try to parse the content as an expression
                        if let Ok(replacement_expr) = syn::parse2::<Expr>(tokens.clone())
						{
                            //eprintln!("Successfully parsed replacement expression");
                            *expr = replacement_expr;
                        } else {
                            //eprintln!("Failed to parse replacement expression");
                        }
                        return; // Don't visit recursively if we found insert_here!
                    }
                }
                // Visit the expression recursively if it's not insert_here!
                self.visit_expr_mut(expr);
            }
            Stmt::Local(_) =>
			{
                //eprintln!("Found Stmt::Local (let binding)");
                syn::visit_mut::visit_stmt_mut(self, stmt);
            }
            Stmt::Item(_) =>
			{
                //eprintln!("Found Stmt::Item");
                syn::visit_mut::visit_stmt_mut(self, stmt);
            }
            Stmt::Macro(macro_stmt) =>
			{
                //eprintln!("Found Stmt::Macro: {}", macro_stmt.mac.path.get_ident().map_or("none".to_string(), |i| i.to_string()));
                if macro_stmt.mac.path.is_ident("insert_here")
				{
                    //eprintln!("Found insert_here! in Stmt::Macro context");
                    let tokens = &macro_stmt.mac.tokens;
                    //eprintln!("Tokens: {}", tokens);
                    
                    // First try to parse as a statement (handles cases with semicolons inside)
                    if let Ok(replacement_stmt) = syn::parse2::<Stmt>(tokens.clone())
					{
                        //eprintln!("Successfully parsed replacement as statement");
                        *stmt = replacement_stmt;
                        return;
                    }
                    
                    // If that fails, try to parse as an expression and convert to Stmt::Expr
                    if let Ok(replacement_expr) = syn::parse2::<Expr>(tokens.clone())
					{
                        //eprintln!("Successfully parsed replacement as expression");
                        *stmt = Stmt::Expr(replacement_expr, macro_stmt.semi_token.clone());
                        return;
                    }
                    
                    //eprintln!("Failed to parse replacement content");
                    return;
                }
                syn::visit_mut::visit_stmt_mut(self, stmt);
            }
        }
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr)
	{
        if let Expr::Macro(macro_expr) = expr
		{
            if macro_expr.mac.path.is_ident("insert_here")
			{
                let tokens = &macro_expr.mac.tokens;
                
                // Try to parse the content as an expression
                if let Ok(replacement_expr) = syn::parse2::<Expr>(tokens.clone())
				{
                    *expr = replacement_expr;
                    return; // Don't visit recursively
                }
            }
        }
        
        // Continue visiting nested expressions
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
